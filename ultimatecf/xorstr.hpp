#pragma once

/////////////////////////////////////////////////////////
/// \tparam _t character type
/// \tparam _s size of string
/// \tparam _k key
/////////////////////////////////////////////////////////
template< class _t, const size_t _s, const _t _k = 'x' >
class xstr {
	/////////////////////////////////////////////////////////
	/// \brief stores string that will be
	///        encrypted at compile-time and
	///        decrypted at run-time
	/////////////////////////////////////////////////////////
	std::array< _t, _s > buffer;

	/////////////////////////////////////////////////////////
	/// \brief    used to encrypt character at compile-time
	///
	/// \param ch character to encrypt
	///
	/// \return   encrypted character
	/////////////////////////////////////////////////////////
	constexpr auto enc(const _t ch) const noexcept -> _t {
		return ch ^ _k;
	}

	/////////////////////////////////////////////////////////
	/// \brief    used to decrypt character at run-time
	///
	/// \param ch character to decrypt
	///
	/// \return   decrypted character
	/////////////////////////////////////////////////////////
	auto dec(const _t ch) const noexcept -> _t {
		return ch ^ _k;
	}

public:
	/////////////////////////////////////////////////////////
	/// \return decrypted buffer
	/////////////////////////////////////////////////////////
	auto data() noexcept {
		for (auto i = _s; i--; ) {
			buffer[i] = dec(buffer[i]);
		}

		return buffer.data();
	}

	/////////////////////////////////////////////////////////
	/// \brief     default class constructor
	///
	/// \tparam _i character index
	///
	/// \param  s  string to be encrypted
	/////////////////////////////////////////////////////////
	template< size_t... _i >
	constexpr __forceinline xstr(const _t(&s)[_s], std::index_sequence< _i... >) noexcept
		: buffer{ enc(s[_i])... } {}
};

/////////////////////////////////////////////////////////
/// \tparam _t character type
/// \tparam _s size of string
///
/// \param s   plain string that will be encrypted at
///            compile-time and decrypted at run-time
///
/// \return    decrypted string
/////////////////////////////////////////////////////////
template< class _t, size_t _s >
constexpr __forceinline auto Xor(const _t(&s)[_s]) {
	return xstr< _t, _s >{ s, std::make_index_sequence< _s >() }.data();
}

#define XorStr(x) Xor(x)