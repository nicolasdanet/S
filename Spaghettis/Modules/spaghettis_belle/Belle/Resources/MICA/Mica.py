#! /usr/bin/env python3

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Copyright (c) 2017 Jojo and others.

# < http://opensource.org/licenses/BSD-3-Clause >

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

# Create MICA .hpp files from the XML library.
# This file can NOT be edited without considering the CPP part also.
# For now, only the english language is supported.

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

import uuid
import xml.etree.ElementTree as ET
from pathlib import Path

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

class Generate:

    @staticmethod
    def _keyIsNumber(s):
        try: 
            int(s)
            return True
        except ValueError:
            return False
    
    @staticmethod
    def _uuidIsNumber(uuid):
        if uuid.hex[16:] == "0000000000000001":     # Can NOT be true for a valid UUID version 3.
            return True
        else:
            return False
    
    # To be combined UUIDs are sorted then merged using a simple PRNG.
    # Assume that no collision occurs.
    # http://www.codeproject.com/Articles/25172/Simple-Random-Number-Generation
    
    @staticmethod
    def _hashHalf(hex1, hex2):
    
        low16 = int(0xffff)
        low32 = int(0xffffffff)
        a = int(hex1, 16)
        b = int(hex2, 16)
        
        zH = 36969 * ((a >> 32) & low16) + (a >> 48)
        wH = 18000 * ((b >> 32) & low16) + (b >> 48)
        zL = 36969 * (a & low16) + ((a & low32) >> 16)
        wL = 18000 * (b & low16) + ((b & low32) >> 16)
        
        # Must use a mask to avoid integer bit-width expansion.
        
        return (((((zL << 16) + wL) & low32) + (((zH << 16) + wH) << 32)) & 0xffffffffffffffff)
        
    @staticmethod
    def _hash(uuid1, uuid2):
        high = Generate._hashHalf(uuid1.hex[:16], uuid2.hex[:16]).to_bytes(8, byteorder='big')
        low  = Generate._hashHalf(uuid1.hex[16:], uuid2.hex[16:]).to_bytes(8, byteorder='big')
        return uuid.UUID(bytes=high + low)
    
    @staticmethod
    def uuidHash(mapping):
        toHash = sorted(mapping, key=lambda x: x.int)
        hashed = toHash.pop(0)
        for h in toHash:
            hashed = Generate._hash(hashed, h)
        return hashed
    
    # Concept is labeled with an unique UUID version 3.
    # It is based onto the english identifier.
    # Integers are handled as ratio (with a denominator equal to 1).

    @staticmethod
    def uuidMake(key):
        if key == "en:Undefined":
            return uuid.UUID(bytes=b'\x00' * 16)
        elif Generate._keyIsNumber(key):
            high = int(key).to_bytes(8, byteorder='big', signed=True)
            low  = b'\x00\x00\x00\x00\x00\x00\x00\x01'
            return uuid.UUID(bytes=high + low)
        else:
            return uuid.uuid3(uuid.UUID(bytes=b'\x31\xca' * 8), key)
    
    # Ensure that a concept has an unique uuid.
    
    @staticmethod
    def uuidUnique(uuid, uuids):
        if uuid in uuids:
            return False
        else:
            uuids.add(uuid)
            return True
            
    @staticmethod
    def uuidExist(uuid, uuids):
        if Generate._uuidIsNumber(uuid):
            return True
        elif uuid in uuids:
            return True
        else:
            return False
    
    # Representation with to unsigned 64-bit integers for the CPP headers.
    
    @staticmethod
    def uuidAsString(uuid):
        s = [ "{ ", "0x", uuid.hex[:16], "ULL, ", "0x", uuid.hex[16:], "ULL }" ]
        return "".join(s)
        
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

class Concept:
    
    def __init__(self, element):
        self.id = None
        self.uuid = None
        self.definition = None
        self.sequence = None
        self.cyclic = False
        
        identifier = element.find("identifier")
        if identifier is not None:
            if identifier.get("lang") == "en":
                self.id = identifier.text
                self.uuid = Generate.uuidMake("en:" + identifier.text)
        
        definition = element.find("definition")
        if definition is not None:
            if definition.get("lang") == "en":
                self.definition = definition.get("name")
        
        sequence = element.find("sequence")
        if sequence is not None:
            self.sequence = sequence.text
            if sequence.get("cyclic") == "true":
                self.cyclic = True
    
    # A negative length means that the sequence is cyclic.
    
    def length(self):
        if self.sequence is not None:
            size = len(self.sequence.split())
            if self.cyclic:
                size = size * -1
            return size
        else:
            return 0
    
    # Concept must be complete with an unique identifier.
    
    def isValid(self, uuids):
        if None in (self.id, self.uuid, self.definition):
            return False
        else:
            return Generate.uuidUnique(self.uuid, uuids)
    
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

class Builder:

    def __init__(self):
        self.uuids = set()
        self.concepts = []
        self.maps = []
        self.items = []
        self.indexes = []
        self.hashes = []
        
    def _parseCategory(self, child):
        for element in child:
            if element.tag == "concept":
                concept = Concept(element)
                if concept.isValid(self.uuids):
                    self.concepts.append(concept)
                else:
                    print(concept)
                    raise RuntimeError("Invalid concept")
        
    def _parseMap(self, child):
        self.maps.append(child.text)

    def _parseRoot(self, root):
        if root.tag == "mica":
            for child in root:
                if child.tag == "map":
                    self._parseMap(child)
                elif child.tag == "category":
                    self._parseCategory(child)
    
    # If a sequence contains repeated concepts the indexes cannot be lookup.
    # Indeed the binary search algorithm used can not properly manage identical entries.
    
    def _buildIndexes(self):
        for concept in self.concepts:
            if concept.sequence is not None:
                sequence = list(map(Generate.uuidMake, concept.sequence.split()))
                repeated = (len(set(sequence)) != len(sequence))
                for i, e in enumerate(sequence):
                    if Generate.uuidExist(e, self.uuids):
                        if not repeated:
                            self.items.append((concept.uuid, e, i))
                        self.indexes.append((concept.uuid, i, e))
                    else:
                        print(concept.sequence)
                        raise RuntimeError("Invalid sequence")
    
    # UUIDs are collapsed to make the search key.
    
    def _buildMaps(self):
        for line in self.maps:
            mapping = list(map(Generate.uuidMake, line.split()))
            if len(mapping) > 1:
                result = mapping.pop()
                hashed = Generate.uuidHash(mapping)
                if Generate.uuidExist(result, self.uuids):
                    if Generate.uuidUnique(hashed, self.uuids):
                        self.hashes.append((hashed, result))
                        continue
                    
            print(line)
            raise RuntimeError("Invalid map")
            
    def parse(self, path):
    
        for file in path.glob('**/*.xml'):
            self._parseRoot(ET.parse(str(file)).getroot())
            
        self._buildIndexes()
        self._buildMaps()
        
        return self

# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

    def writeConcepts(self):
    
        self.concepts.sort(key=lambda x: x.id)
        
        with open("./MicaDatabase.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace mica {\n")
            f.write("\n")
            
            for concept in self.concepts:
                f.write("extern const UUID ")
                f.write(concept.id)
                f.write(";\n")
                
            f.write("\n")
            f.write("} // namespace mica")
            f.write("\n")
    
        with open("./MicaData_1.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace mica {\n")
            f.write("\n")
            
            for concept in self.concepts:
                f.write("const UUID ")
                f.write(concept.id.ljust(44))
                f.write(" = ")
                f.write(Generate.uuidAsString(concept.uuid))
                f.write(";\n")
                
            f.write("\n")
            f.write("} // namespace mica")
            f.write("\n")
    
    def writeKeyToConcept(self):
        
        self.concepts.sort(key=lambda x: x.definition)
    
        with open("./MicaData_2.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace internal {\n")
            f.write("\n")
        
            f.write("static const int keyToConceptSize = ")
            f.write(str(len(self.concepts)))
            f.write(";\n")
            f.write("static const KeyToConcept keyToConceptLookup[keyToConceptSize] = {\n")

            for i, concept in enumerate(self.concepts):
                if i:
                    f.write(",\n")
                f.write("{ \"en:")
                f.write("".join((concept.definition, "\",")).ljust(50))
                f.write(Generate.uuidAsString(concept.uuid))
                f.write(" }")
            
            f.write("\n")
            f.write("};\n")
            f.write("\n")
            f.write("} // namespace internal")
            f.write("\n")
    
    def writeConceptToKey(self):
        
        self.concepts.sort(key=lambda x: x.uuid.int)
        
        with open("./MicaData_3.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace internal {\n")
            f.write("\n")
        
            f.write("static const int conceptLanguageToKeySize = ")
            f.write(str(len(self.concepts)))
            f.write(";\n")
            f.write("static const ConceptLanguageToKey"
                " conceptLanguageToKeyLookup[conceptLanguageToKeySize] = {\n")
        
            for i, concept in enumerate(self.concepts):
                if i:
                    f.write(",\n")
                f.write("{ ")
                f.write(Generate.uuidAsString(concept.uuid))
                f.write(", \"en\", \"en:")
                f.write("".join((concept.definition, "\"")).ljust(40))
                f.write("}")
                
            f.write("\n")
            f.write("};\n")
            f.write("\n")
            f.write("} // namespace internal")
            f.write("\n")
    
        with open("./MicaData_4.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace internal {\n")
            f.write("\n")
        
            f.write("static const int conceptToLengthSize = ")
            f.write(str(len(self.concepts)))
            f.write(";\n")
            f.write("static const ConceptToLength conceptToLengthLookup[conceptToLengthSize] = {\n")
            
            for i, concept in enumerate(self.concepts):
                if i:
                    f.write(",\n")
                f.write("{ ")
                f.write(Generate.uuidAsString(concept.uuid))
                f.write(",")
                f.write("".join((str(concept.length()), " }")).rjust(7))
                
            f.write("\n")
            f.write("};\n")
            f.write("\n")
            f.write("} // namespace internal")
            f.write("\n")
    
    def writeConceptToIndex(self):
    
        self.items.sort(key=lambda x: (x[0].int, x[1].int))
        
        with open("./MicaData_5.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace internal {\n")
            f.write("\n")
        
            f.write("static const int sequenceConceptToIndexSize = ")
            f.write(str(len(self.items)))
            f.write(";\n")
            f.write("static const SequenceConceptToIndex"
                " sequenceConceptToIndexLookup[sequenceConceptToIndexSize] = {\n")
            
            for i, item in enumerate(self.items):
                if i:
                    f.write(",\n")
                f.write("{ ")
                f.write(Generate.uuidAsString(item[0]))
                f.write(", ")
                f.write(Generate.uuidAsString(item[1]))
                f.write(",")
                f.write(str(item[2]).rjust(5))
                f.write(" }")
                
            f.write("\n")
            f.write("};\n")
            f.write("\n")
            f.write("} // namespace internal")
            f.write("\n")

    def writeIndexToConcept(self):
    
        self.indexes.sort(key=lambda x: (x[0].int, x[1]))
        
        with open("./MicaData_6.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace internal {\n")
            f.write("\n")
        
            f.write("static const int sequenceIndexToConceptSize = ")
            f.write(str(len(self.indexes)))
            f.write(";\n")
            f.write("static const SequenceIndexToConcept"
                " sequenceIndexToConceptLookup[sequenceIndexToConceptSize] = {\n")
            
            for i, index in enumerate(self.indexes):
                if i:
                    f.write(",\n")
                f.write("{ ")
                f.write(Generate.uuidAsString(index[0]))
                f.write(",")
                f.write(str(index[1]).rjust(5))
                f.write(", ")
                f.write(Generate.uuidAsString(index[2]))
                f.write(" }")
                
            f.write("\n")
            f.write("};\n")
            f.write("\n")
            f.write("} // namespace internal")
            f.write("\n")
        
    def writeMapToConcept(self):
    
        self.hashes.sort(key=lambda x: x[0].int)
        
        with open("./MicaData_7.hpp", "w") as f:
            f.write("\n")
            f.write("/* Auto-generated file. */\n")
            f.write("\n")
            f.write("// ====================================\n")
            f.write("\n")
            f.write("namespace internal {\n")
            f.write("\n")
        
            f.write("static const int mapToConceptSize = ")
            f.write(str(len(self.hashes)))
            f.write(";\n")
            f.write("static const MapToConcept mapToConceptLookup[mapToConceptSize] = {\n")
            
            for i, mapped in enumerate(self.hashes):
                if i:
                    f.write(",\n")
                f.write("{ ")
                f.write(Generate.uuidAsString(mapped[0]))
                f.write(", ")
                f.write(Generate.uuidAsString(mapped[1]))
                f.write(" }")
                
            f.write("\n")
            f.write("};\n")
            f.write("\n")
            f.write("} // namespace internal")
            f.write("\n")
            
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------

if __name__ == "__main__":
    builder = Builder().parse(Path("./Library"))
    builder.writeConcepts()
    builder.writeKeyToConcept()
    builder.writeConceptToKey()
    builder.writeConceptToIndex()
    builder.writeIndexToConcept()
    builder.writeMapToConcept()
    
# ------------------------------------------------------------------------------------------------------------
# ------------------------------------------------------------------------------------------------------------
