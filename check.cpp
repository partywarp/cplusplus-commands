#include "check.hpp"
std::string collect(std::istream &is)
{
	std::string contents, current_token;
	for (is >> contents; is >> current_token;) {
		contents += ' ';
		contents += current_token;
	}
	return contents;
}
bool check(std::istream& real, std::istream& expect)
{
	bool prefix_matched = false;
	std::string last_expect_token;
	std::string last_recv_token;
	while (true) {
		std::string expected, received;
		if ((expect >> expected).fail() !=
			    (real >> received).fail() ||
		    !real.fail() && expected != received) {
			last_expect_token = expected;
			last_recv_token = received;
			break;
		}
		if (real.fail()) {
			return true;
		}
		prefix_matched = true;
		last_expect_token = expected;
		last_recv_token = received;
	}
	// allow stdin to passthrough + print expected output
	std::string rest_expected = (prefix_matched ? "<same> " : "") +
				    last_expect_token + " " + collect(expect);
	std::string rest_received = (prefix_matched ? "<same> " : "") +
				    last_recv_token + " " + collect(real);
	int width = 120;
	int receive_step_size = width / 2 - 3;
	int expect_step_size = width - width / 2;
	for (int i = 0, j = 0;
	     i < rest_received.size() || j < rest_expected.size();
	     i += receive_step_size, j += expect_step_size) {
		printf("%-*s │ %s\n", receive_step_size,
		       i < rest_received.size() ?
			       rest_received.substr(i, receive_step_size)
				       .c_str() :
			       "",
		       j < rest_expected.size() ?
			       rest_expected.substr(j, expect_step_size).c_str() :
			       "");
	}
	return false;
}
