//
// Created by alex on 27.08.22.
//

#ifndef AGE_MOUSECODES_H
#define AGE_MOUSECODES_H

#include "agepch.h"

namespace AGE {
	using MouseCode = uint8_t;

	namespace Mouse {
		enum : MouseCode {
			//@formatter:off
			Button0       = 0,
			Button1       = 1,
			Button2       = 2,
			Button3       = 3,
			Button4       = 4,
			Button5       = 5,
			Button6       = 6,
			Button7       = 7,

			ButtonLast    = Button7,
			ButtonLeft    = Button0,
			ButtonRight   = Button1,
			ButtonMiddle  = Button2
		//@formatter:on
		};
	}
}

#endif //AGE_MOUSECODES_H
