#include "GF.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include <random>

template<class F>
F lagrange_interpolation(std::vector<F> xs, std::vector<F> ys, F x_0){
	F res{0};
	assert(xs.size() == ys.size());
	for(int i=0; i<xs.size(); ++i){
		auto f_i =
			[=](F x){
				F pi{1};
				for(int k=0; k<xs.size(); ++k)
					if(i != k)
						pi *= (x - xs[k]);
				return pi;
			};
		res += ys[i] * f_i(x_0) / f_i(xs[i]);
	}
	return res;
}

int main(){
	std::random_device seed_gen;
	std::mt19937 rand(seed_gen());
	std::uniform_int_distribution<> dist(0, 307);

	// secret
	std::array<GF<307>, 3> s{62, 85, 57};

	// make random (private) key
	std::array<std::array<GF<307>, 2>, 3> key{};
	for(int from = 0; from < 3; ++from)
		for(int coeff = 0; coeff < 2; ++coeff)
			key[from][coeff] = GF<307>{dist(rand)};

	std::cout << "A = " << s[0] << ", B = " << s[1] << ", C = " << s[2] << std::endl;
	std::cout << "DEBUG: Sum{ A, B, C } = " << s[0]+s[1]+s[2] << std::endl;
	std::cout << "DEBUG: Average{ A, B, C } = " << int{s[0]+s[1]+s[2]} / 3.0 << std::endl;

	std::cout << "----------" << std::endl;

	// calculate "share" and send
	std::array<std::array<GF<307>, 3>, 3> share;  // [from][to]
	for(int from = 0; from < 3; ++from){
		for(int to = 0; to < 3; ++to){
			// key_0 * x^2 + key_1 * x + s[from]
			share[from][to] =
				  key[from][0] * GF<307>{(to+1)*(to+1)}
				+ key[from][1] * GF<307>{(to+1)}
				+ s[from];
			std::cout << "f_" << char('A' + from) << "(" << to + 1 << ") = " << share[from][to] << ", ";
		}
		std::cout << std::endl;
	}

	std::cout << "----------" << std::endl;

	// received "share"
	std::array<GF<307>, 3> received_sum{};
	for(int to = 0; to < 3; ++to){
		received_sum[to] = share[0][to] + share[1][to] + share[2][to];
		std::cout
			<< char('A' + to) << " received: "
			<< share[0][to] << " + "
			<< share[1][to] << " + "
			<< share[2][to] << " = "
			<< received_sum[to] << std::endl;
	}

	std::cout << "----------" << std::endl;

	// matching
	for(int i=0; i<3; ++i){
		std::cout << "f_sum(" << i+1 << ") = " << received_sum[i] << std::endl;
	}

	std::cout << "----------" << std::endl;

	GF<307> calculated_s =
		lagrange_interpolation<GF<307>>(
			{1,2,3},
			{received_sum[0],received_sum[1],received_sum[2]},
			0
		);

	std::cout << "f_sum(0) = " << calculated_s << std::endl;
	std::cout << "average = " << int{calculated_s} / 3.0 << std::endl;

	return 0;
}
