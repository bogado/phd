#pragma once

#ifndef PHD_TEXT_BASIC_C_STRING_VIEW_HPP
#define PHD_TEXT_BASIC_C_STRING_VIEW_HPP

#include <phd/meta/remove_cv_ref.hpp>
#include <phd/meta/always.hpp>
#include <phd/text/char8_t.hpp>
#include <phd/text/text_forward.hpp>

#include <string_view>
#include <type_traits>
#include <cassert>

namespace phd {

	namespace string_detail {

		template <typename C>
		constexpr inline decltype(auto) empty_string() {
			static_assert(meta::always_false_v<C>, "unrecognized character type");
			return "";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char>() {
			return "";
		}

		template <>
		constexpr inline decltype(auto) empty_string<wchar_t>() {
			return L"";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char8_t>() {
			return u8"";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char16_t>() {
			return u"";
		}

		template <>
		constexpr inline decltype(auto) empty_string<char32_t>() {
			return U"";
		}
	} // namespace string_detail

	template <typename CharType, typename Traits = std::char_traits<CharType>>
	class basic_c_string_view : public std::basic_string_view<CharType, Traits> {
	private:
		using base_t = std::basic_string_view<CharType, Traits>;

	public:
		constexpr basic_c_string_view()
		: basic_c_string_view(string_detail::empty_string<CharType>(), 0) {
		}

		template <typename Arg0, typename Arg1, typename... Args>
		constexpr basic_c_string_view(Arg0&& arg0_, Arg1&& arg1_, Args&&... args_)
		: base_t(std::forward<Arg0>(arg0_), std::forward<Arg1>(arg1_), std::forward<Args>(args_)...) {
			assert((this->data() ? *(this->data() + this->size()) == '\0' : false) && "non null-terminated c_string_view!");
		}

		template <typename Arg, typename = std::enable_if_t<!std::is_same_v<meta::remove_cv_ref_t<Arg>, basic_c_string_view>>>
		constexpr basic_c_string_view(Arg&& arg_)
		: base_t(std::forward<Arg>(arg_)) {
			assert((this->data() ? *this->end() == '\0' : false) && "non null-terminated c_string_view!");
		}

		constexpr basic_c_string_view(basic_c_string_view&& o) = default;
		constexpr basic_c_string_view(const basic_c_string_view& o) = default;
		constexpr basic_c_string_view& operator=(basic_c_string_view&& o) = default;
		constexpr basic_c_string_view& operator=(const basic_c_string_view& o) = default;
	};

	using c_string_view = basic_c_string_view<char>;
	using wc_string_view = basic_c_string_view<wchar_t>;
	using u8c_string_view = basic_c_string_view<char8_t>;
	using u16c_string_view = basic_c_string_view<char16_t>;
	using u32c_string_view = basic_c_string_view<char32_t>;

	inline namespace literals {
	inline namespace string_view_literals {
		// suffix for basic_c_string_view literals
		constexpr inline c_string_view operator"" _csv(const char* str_, size_t len_) noexcept {
			return c_string_view(str_, len_);
		}
#if defined(__cpp_char_8t)
		constexpr inline u8c_string_view operator"" _csv(const char8_t* str_, size_t len_) noexcept {
			return u8c_string_view(str_, len_);
		}
#else
		constexpr inline u8c_string_view operator"" _cu8sv(const char* str_, size_t len_) noexcept {
			return u8c_string_view(reinterpret_cast<const char8_t*>(str_), len_);
		}
#endif // char8_t hacks
		constexpr inline u16c_string_view operator"" _csv(const char16_t* str_, size_t len_) noexcept {
			return u16c_string_view(str_, len_);
		}
		constexpr inline u32c_string_view operator"" _csv(const char32_t* str_, size_t len_) noexcept {
			return u32c_string_view(str_, len_);
		}
		constexpr inline wc_string_view operator"" _csv(const wchar_t* str_, size_t len_) noexcept {
			return wc_string_view(str_, len_);
		}
	}} // namespace literals::string_view_literals

} // namespace phd

namespace std {

	template <typename CharType, typename Traits>
	struct hash<::phd::basic_c_string_view<CharType, Traits>> {

		size_t operator()(const ::phd::basic_c_string_view<CharType, Traits>& c_string_) const {
			hash<basic_string_view<CharType, Traits>> h;
			return h(c_string_);
		}
	};

} // namespace std

#endif // PHD_TEXT_BASIC_C_STRING_VIEW_HPP