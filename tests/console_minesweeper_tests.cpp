

#include <gtest/gtest.h>

#include <console-minesweeper/game.h>

class ConsoleMinesweeperTest : public ::testing::Test {
protected:

	// used for setting up fields for testing
	void SetUp() override {

		this->setUpField1();		
	}

	void setUpField1() {

		field1 = 13;
	}




	// declarations:

	int field1;
};

using ConsoleMinesweeperDeathTest = ConsoleMinesweeperTest;

TEST_F(ConsoleMinesweeperDeathTest, test_name) { // TODO


	EXPECT_EQ(1, 1) << "Test failed message.";

	EXPECT_EQ(0, 0);

}

TEST_F(ConsoleMinesweeperDeathTest, another_test_name) { // TODO

	int x = 1;
	EXPECT_EQ(x, 1) << "Test failed message.";

	EXPECT_EQ(0, 0);

}