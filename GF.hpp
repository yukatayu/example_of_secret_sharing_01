#pragma once
#include <array>
#include <stdexcept>
#include <cassert>

template<int q>
struct GF {
private:
	// utilities
	constexpr static int mod(int x) {
		return ((x % q) + q) % q;
	}

	constexpr static bool is_prime(int x){
		if(q < 2)
			return false;
		for(int i=2; i*i <= q; ++i)
			if(q % i == 0)
				return false;
		return true;
	}

	constexpr static GF<q> pow(GF<q> x, int n){
		assert(n >= 0);
		GF<q> res{1};
		for(; n > 0; x *= x, n >>= 1)
			if(n & 1)
				res *= x;
		return res;
	}

	// internal variables
	int val_{};

public:
	int val(){ return val_; }
	operator int(){ return val_; }

	GF(int val = 0) : val_(mod(val)) {
		// check if q is prime
		static_assert(is_prime(q), "The order of GF must be prime.");
	}

	GF<q> inv() const {
		// Fermat's little theorem
		return pow(*this, q-2);
	}

	GF<q> operator-() const { return GF<q>(-val_); }

	GF<q>& operator+=(const GF<q> &rhs) {
		val_ = mod(val_ + rhs.val_);
		return *this;
	}

	GF<q>& operator-=(const GF<q> &rhs) {
		val_ = mod(val_ - rhs.val_);
		return *this;
	}

	GF<q>& operator*=(const GF<q> &rhs) {
		val_ = mod(val_ * rhs.val_);
		return *this;
	}

	GF<q>& operator/=(const GF<q> &rhs) {
		val_ = mod(val_ * rhs.inv().val_);
		return *this;
	}

	GF<q> operator+(const GF<q> &rhs) const {
		GF<q> res = *this;
		res += rhs;
		return res;
	}

	GF<q> operator-(const GF<q> &rhs) const {
		GF<q> res = *this;
		res -= rhs;
		return res;
	}

	GF<q> operator*(const GF<q> &rhs) const {
		GF<q> res = *this;
		res *= rhs;
		return res;
	}

	GF<q> operator/(const GF<q> &rhs) const {
		GF<q> res = *this;
		res /= rhs;
		return res;
	}
};
