
/* Copyright (c) 2024 Jojo and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
// MARK: -

namespace spaghettis {

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

std::unique_ptr<PainterStrategy> Painter::createPainter (ObjectComponent* owner, const juce::String& type)
{
    if (type == "bng")          { return std::make_unique<BangPainter> (owner);     }
    else if (type == "comment") { return std::make_unique<CommentPainter> (owner);  }
    else if (type == "gatom")   { return std::make_unique<AtomPainter> (owner);     }
    else if (type == "garray")  { return std::make_unique<ArrayPainter> (owner);    }
    else if (type == "inlet")   { return std::make_unique<InletPainter> (owner);    }
    else if (type == "message") { return std::make_unique<MessagePainter> (owner);  }
    else if (type == "outlet")  { return std::make_unique<InletPainter> (owner);    }
    else if (type == "tgl")     { return std::make_unique<TogglePainter> (owner);   }
    else if (type == "hradio")  { return std::make_unique<RadioPainter> (owner);    }
    else if (type == "hslider") { return std::make_unique<SliderPainter> (owner);   }
    else if (type == "dial")    { return std::make_unique<DialPainter> (owner);     }
    else if (type == "vu")      { return std::make_unique<VuPainter> (owner);       }
    else {
        return std::make_unique<BoxPainter> (owner);
    }
}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

}

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
