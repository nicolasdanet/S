
/* Copyright (c) 2022 Spaghettis and others. */

/* < https://www.gnu.org/licenses/agpl-3.0.en.html > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

struct ZoomSteps {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static constexpr std::array<int, 16> steps_ =
    {
         30,
         50,
         70,
         80,
         90,
        100,
        110,
        120,
        130,
        150,
        170,
        200,
        240,
        300,
        400,
        500
    };
    
// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

static constexpr int min()
{
    return steps_.front();
}

static constexpr int max()
{
    return steps_.back();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

static int next (int step)
{
    auto r = std::find_if (steps_.cbegin(), steps_.cend(),   [n = step](int i) { return (i > n); });

    return (r != steps_.cend()) ? *r : steps_.back();
}

static int previous (int step)
{
    auto r = std::find_if (steps_.crbegin(), steps_.crend(), [n = step](int i) { return (i < n); });

    return (r != steps_.crend()) ? *r : steps_.front();
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
