
/* "Things Ain't What They Used To Be", words by Ted Person, music by Duke Ellington. */

// ====================================

struct Example_2 {

Example_2() : score_ (mica::CMajor, mica::CutTime)
{
    using namespace prim;
    using namespace mica;
    using namespace belle;
    
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (Beginning).tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (Middle).tuplet (Middle)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (End).tuplet (End)));
    
    score_.lower (score_->addChord (Eighth().rest())).lower (score_->addChord (Quarter().rest()));
    score_.lower().lower();
    
    for (int i = 0; i < 2; ++i) {
    //
    score_.brace (score_->addBarline (StandardBarline));
    
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5)));
    score_.upper (score_->addChord (Eighth().rest()));
    score_.upper (score_->addChord (Quarter().rest()));
    score_.upper();
    score_.upper (score_->addChord (Eighth().tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (Beginning).tuplet (Middle)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (End).tuplet (End)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (Beginning).tuplet (Beginning)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (Middle).tuplet (Middle)));
    score_.upper (score_->addChord (Eighth().notes (D4, F4, A4, C5).beam (End).tuplet (End)));
        
    score_.lower (score_->addChord (Eighth().rest()));
    score_.lower (score_->addChord (Eighth().notes (G2, G3)));
    score_.lower (score_->addChord (Eighth().notes (G2, G3).tuplet (Beginning)));
    score_.lower (score_->addChord (Quarter().tied (G2, G3, Beginning).tuplet (End)));
    score_.lower (score_->addChord (Half().tied (G2, G3, End)));
    score_.lower().lower().lower().lower().lower();
    //
    }
}

public:
    operator belle::MusicBase&() { return score_.close(); }

private:
    belle::GrandStaff score_;
};
