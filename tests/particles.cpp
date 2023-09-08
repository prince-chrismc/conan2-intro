#include "particles.hpp"
#include <gtest/gtest.h>

TEST(particles, inital) {

    PARTICLE p;
    init_particle(&p, 0);

	ASSERT_EQ(1, p.active);
	ASSERT_EQ(1.0f, p.life);
}
