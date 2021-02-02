
/* "Things Ain't What They Used To Be", words by Ted Person, music by Duke Ellington. */

// ====================================

struct Example_3 {

Example_3() : score_ (mica::CMajor, mica::CutTime)
{
    using namespace prim;
    using namespace mica;
    using namespace belle;
    
    for (int i = 0; i < 2; ++i) {
    //
    score_.upper (score_->addChord (Quarter().notes (D4, F4, A4, C5)));
    score_.upper();
    score_.upper (score_->addChord (Eighth().tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().tuplet (Middle)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).tuplet (End)));

    score_.lower (score_->addChord (Eighth().rest()));
    score_.lower (score_->addChord (Eighth().notes (G2, G3)));
    score_.lower (score_->addChord (Quarter().notes (G2, G3)));
    score_.lower();
    score_.lower();
    //
    }
    
    score_.brace (score_->addBarline (StandardBarline));
    
    for (int i = 0; i < 3; ++i) {
    //
    score_.upper (score_->addChord (Quarter().notes (D4, F4, A4, C5)));
    score_.upper();
    
    score_.lower (score_->addChord (Eighth().rest()));
    score_.lower (score_->addChord (Eighth().notes (G2, G3)));
    //
    }
    
    score_.upper (score_->addChord (Quarter().notes (F3, AFlat3, B3, EFlat4).tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().notes (E3, G3, A3, C4).tuplet (End)));
    
    score_.lower (score_->addChord (Quarter().note (G2).tuplet (Beginning)));
    score_.lower (score_->addChord (Eighth().note (C2, Beginning).tuplet (End)));
    
    score_.brace (score_->addBarline (DoubleBarline));
    
    score_.upper (score_->addChord (Eighth().tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().note (G3).beam (Beginning).tuplet (Middle)));
    score_.upper (score_->addChord (Eighth().note (C4).beam (End).tuplet (End)));
    score_.upper (score_->addChord (Eighth().note (E4).beam (Beginning).tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().note (G4).beam (Middle).tuplet (Middle)));
    score_.upper (score_->addChord (Eighth().note (A4).beam (End).tuplet (End)));
    score_.upper (score_->addChord (Quarter().note (G4)));
    score_.upper (score_->addChord (Eighth().note (A4)));
    score_.upper (score_->addChord (Eighth().rest()));
    
    score_.lower (score_->addChord (Quarter().note (C2, End)));
    score_.lower();
    score_.lower();
    score_.lower (score_->addChord (Quarter().note (A2)));
    score_.lower();
    score_.lower();
    score_.lower (score_->addChord (Quarter().note (G2)));
    score_.lower (score_->addChord (Quarter().note (F2)));
    score_.lower();
}

public:
    operator belle::MusicBase&() { return score_.close(); }

private:
    belle::GrandStaff score_;
};
