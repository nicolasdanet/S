
/* Copyright (c) 2023 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis::core {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/*

s[0] ? comment | text       -> text         0 0  s[1] ...
s[0] ? message | msg        -> msg          0 0  s[1] ...
s[0] ? gatom   | floatatom  -> floatatom    0 0  s[1] ...
                            -> obj          0 0  s[0] s[1] ...
                            
msg          36 108      hello;
text         24 24       widgets;
floatatom    36 192      5 0 0;
obj          276 120     garray;

*/

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

void fillBufferAppendAtom (t_buffer *b, const juce::String& s)
{
    t_atom a;
    
    t_error err = atom_withStringUnzeroed (&a, s.toRawUTF8(), static_cast<int> (s.getNumBytesAsUTF8()));
    
    PD_UNUSED (err); PD_ASSERT (!err);
    
    if (!IS_SEMICOLON_OR_COMMA (&a)) { buffer_appendAtom (b, &a); }
}
        
void fillBuffer (t_buffer *b,
    t_symbol *s,
    const juce::Point<int>& pt,
    const juce::StringArray& t,
    int offset)
{
    const int n = t.size();
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, s);
    buffer_appendFloat (b, pt.getX());
    buffer_appendFloat (b, pt.getY());
    
    for (int i = offset; i < n; ++i) { fillBufferAppendAtom (b, t[i]); }
    
    buffer_appendSemicolon (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parser::Parser (const juce::Point<int>& pt, const juce::String& s) : b_ (buffer_new())
{
    const juce::StringArray t (juce::StringArray::fromTokens (s, true));
    
    if (!t.isEmpty()) {
    //
    const juce::String c = t[0];
    
    if (c == Tag::comment       || c == Tag::text)      { fillBuffer (b_, sym_text,      pt, t, 1); }
    else if (c == Tag::message  || c == Tag::msg)       { fillBuffer (b_, sym_msg,       pt, t, 1); }
    else if (c == Tag::gatom    || c == Tag::floatatom) { fillBuffer (b_, sym_floatatom, pt, t, 1); }
    else {
        fillBuffer (b_, sym_obj, pt, t, 0);
    }
    //
    }
}
    
Parser::~Parser()
{
    buffer_free (b_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parser::execute (t_glist *)
{
    char *s = buffer_toString (b_);
    DBG (s);
    PD_MEMORY_FREE (s);
    
    // instance_loadSnippet
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
