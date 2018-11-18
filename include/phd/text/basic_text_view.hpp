#pragma once
#ifndef PHD_TEXT_BASIC_TEXT_VIEW_HPP
#define PHD_TEXT_BASIC_TEXT_VIEW_HPP

#include <phd/text/text_forward.hpp>

namespace phd {

	template <typename Encoding, typename NormalizationForm = nfkc, typename Iterable = basic_c_string_view<code_unit_t<Encoding>>>
	class basic_text_view {
	public:
		using encoding_type = Encoding;
		using iterable_type = Iterable;

	private:
		iterable_type mstorage;

	public:
		iterable_type&& storage() && {
			return std::move(mstorage);
		}

		const iterable_type& storage() const& {
			return mstorage;
		}

		iterable_type& storage() & {
			return mstorage;
		}
	};

} // namespace phd

#endif // PHD_TEXT_BASIC_TEXT_VIEW_HPP