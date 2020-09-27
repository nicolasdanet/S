
/* "Caravan" by Duke Ellington, Irving Mills & Juan Tizol. */

// ====================================

struct Example_1 {

Example_1() : score_ (mica::FMinor, mica::CutTime)
{
    using namespace prim;
    using namespace mica;
    using namespace belle;
    
    for (int i = 0; i < 4; ++i) {
    //
    if (i) { score_.brace (score_->addBarline (StandardBarline)); }
    
    score_.upper (score_->addChord (Quarter().rest()));
    score_.lower (score_->addChord (QuarterDotted().note (C2)));
    score_.upper (score_->addChord (Eighth().rest())).lower();
    score_.upper (score_->addChord (Eighth().notes (G3, BFlat3, DFlat4, E4)));
    score_.lower (score_->addChord (Eighth().rest()));
    
    score_.upper (score_->addChord (Half().rest()));
    score_.lower (score_->addChord (Eighth().rest()));
    score_.upper().lower (score_->addChord (Eighth().note (DFlat3).beam (Beginning)));
    score_.upper().lower (score_->addChord (Eighth().note (C3).beam (Middle)));
    score_.upper().lower (score_->addChord (Eighth().note (G2).beam (End)));
    //
    }
}

public:
    operator belle::MusicBase&() { return score_.close(); }

private:
    belle::GrandStaff score_;
};
