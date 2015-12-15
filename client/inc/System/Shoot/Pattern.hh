#ifndef PATTERN_HH_
# define PATTERN_HH_

# if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#  define _USE_MATH_DEFINES
#  include <ATLComTime.h>
# else
#  include <cmath>
# endif

# include <functional>
# include "System/ASystem.hh"

namespace Pattern {

  enum MovePattern {LINE = 0, SINUS = 1};

  enum class Side {LEFT, RIGHT};

	Pattern::MovePattern incremente(Pattern::MovePattern m);
	void	line(Entity &e, Side s, int duration);
	void	sinusoid(Entity &e, Side s, int duration);
}

#endif
