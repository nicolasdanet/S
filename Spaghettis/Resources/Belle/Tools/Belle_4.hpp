
/* "In A Mellow Tone", By Duke Ellington & Milt Gabler. */

// ====================================

struct Example_4 {

Example_4() : score_ (mica::AFlatMajor, mica::CommonTime)
{
    using namespace prim;
    using namespace mica;
    using namespace belle;
    
    score_.upper (score_->addChord (Eighth().notes (BFlat4, C5, F5, AFlat5)));
    score_.upper (score_->addChord (Quarter().notes (BFlat4, C5, F5, AFlat5)));
    score_.upper (score_->addChord (Eighth().notes (BFlat4, C5, F5, AFlat5)));
    score_.upper (score_->addChord (Eighth().notes (A4, B4, E5, G5)));
    score_.upper (score_->addChord (Eighth().rest()));
    score_.upper (score_->addChord (Eighth().notes (A4, B4, E5, G5).beam (Beginning)));
    score_.upper (score_->addChord (Eighth().notes (BFlat4, C5, F5, AFlat5).beam (End)));
    
    score_.lower (score_->addChord (Eighth().notes (AFlat3, EFlat4, GFlat4)));
    score_.lower (score_->addChord (Quarter().notes (AFlat3, EFlat4, GFlat4)));
    score_.lower (score_->addChord (Eighth().notes (AFlat3, EFlat4, GFlat4)));
    score_.lower (score_->addChord (Eighth().notes (G3, D4, F4)));
    score_.lower (score_->addChord (Eighth().rest()));
    score_.lower (score_->addChord (Eighth().notes (G3, D4, F4).beam (Beginning)));
    score_.lower (score_->addChord (Eighth().notes (AFlat3, EFlat4, GFlat4).beam (End)));
    
    score_.brace (score_->addBarline (StandardBarline));
    
    score_.upper (score_->addChord (Eighth().rest()));
    score_.upper (score_->addChord (Eighth().tied (BFlat4, C5, F5, AFlat5, Beginning)));
    score_.upper (score_->addChord (HalfDotted().tied (BFlat4, C5, F5, AFlat5, End)));
    
    score_.lower (score_->addChord (Eighth().rest()));
    score_.lower (score_->addChord (Eighth().tied (AFlat3, EFlat4, GFlat4, Beginning)));
    score_.lower (score_->addChord (HalfDotted().tied (AFlat3, EFlat4, GFlat4, End)));
    
    score_.brace (score_->addBarline (StandardBarline));
    
    score_.upper (score_->addChord (Eighth().notes (BFlat4, C5, F5, AFlat5)));
    score_.upper (score_->addChord (Quarter().notes (BFlat4, C5, F5, AFlat5)));
    score_.upper (score_->addChord (Eighth().notes (BFlat4, C5, F5, AFlat5)));
    score_.upper (score_->addChord (Eighth().notes (A4, B4, E5, G5)));
    score_.upper (score_->addChord (Eighth().rest()));
    score_.upper (score_->addChord (Eighth().notes (A4, B4, E5, G5).beam (Beginning)));
    score_.upper (score_->addChord (Eighth().notes (BFlat4, C5, F5, AFlat5).beam (End)));
    
    score_.lower (score_->addChord (Eighth().notes (AFlat3, EFlat4, GFlat4)));
    score_.lower (score_->addChord (Quarter().notes (AFlat3, EFlat4, GFlat4)));
    score_.lower (score_->addChord (Eighth().notes (AFlat3, EFlat4, GFlat4)));
    score_.lower (score_->addChord (Eighth().notes (G3, D4, F4)));
    score_.lower (score_->addChord (Eighth().rest()));
    score_.lower (score_->addChord (Eighth().notes (G3, D4, F4).beam (Beginning)));
    score_.lower (score_->addChord (Eighth().notes (AFlat3, EFlat4, GFlat4).beam (End)));
}

public:
    operator belle::MusicBase&() { return score_.close(); }

private:
    belle::GrandStaff score_;
};
