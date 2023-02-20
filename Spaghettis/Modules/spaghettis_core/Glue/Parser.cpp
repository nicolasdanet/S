
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

void fillBufferAppendAtom (t_buffer *t, const juce::String& s)
{
    t_atom a;
    
    t_error err = atom_withStringUnzeroed (&a, s.toRawUTF8(), static_cast<int> (s.getNumBytesAsUTF8()));
    
    PD_UNUSED (err); PD_ASSERT (!err);
    
    if (!IS_SEMICOLON_OR_COMMA (&a)) { buffer_appendAtom (t, &a); }
}
        
void fillBuffer (t_buffer *b,
    t_buffer *t,
    t_symbol *s,
    const juce::Point<int>& pt,
    const juce::StringArray& a,
    int offset)
{
    const int n = a.size();
    
    buffer_appendSymbol (b, sym___hash__X);
    buffer_appendSymbol (b, s);
    buffer_appendFloat (b, pt.getX());
    buffer_appendFloat (b, pt.getY());
    
    for (int i = offset; i < n; ++i) { fillBufferAppendAtom (t, a[i]); }
    
    buffer_serialize (b, t);
    buffer_appendSemicolon (b);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

Parser::Parser (const juce::Point<int>& pt, const juce::String& s) : b_ (buffer_new()), t_ (buffer_new())
{
    const juce::StringArray a (juce::StringArray::fromTokens (s, true));
    
    if (!a.isEmpty()) {
    //
    const juce::String c = a[0];
    
    if (c == Tag::comment       || c == Tag::text)      { fillBuffer (b_, t_, sym_text,      pt, a, 1); }
    else if (c == Tag::message  || c == Tag::msg)       { fillBuffer (b_, t_, sym_msg,       pt, a, 1); }
    else if (c == Tag::gatom    || c == Tag::floatatom) { fillBuffer (b_, t_, sym_floatatom, pt, a, 1); }
    else {
        fillBuffer (b_, t_, sym_obj, pt, a, 0);
    }
    //
    }
}
    
Parser::~Parser()
{
    buffer_free (t_);
    buffer_free (b_);
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

void Parser::execute (t_glist *glist)
{
    if (buffer_getSize (b_)) { instance_loadSnippet (glist, b_); }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
