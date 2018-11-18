#pragma once

#ifndef PHD_OUT_PTR_CLEVER_INOUT_PTR_HPP
#define PHD_OUT_PTR_CLEVER_INOUT_PTR_HPP

#include <phd/out_ptr/detail/clever_inout_ptr_impl.hpp>
#include <phd/version/version.hpp>

#include <cstdlib>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <exception>
#include <utility>
#include <tuple>

namespace phd {

	template <typename Smart, typename Pointer, typename... Args>
	class clever_inout_ptr_t : public out_ptr_detail::clever_inout_ptr_impl<Smart, Pointer, std::tuple<Args...>, std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>> {
	private:
		using list_t = std::make_index_sequence<std::tuple_size_v<std::tuple<Args...>>>;
		using core_t = out_ptr_detail::clever_inout_ptr_impl<Smart, Pointer, std::tuple<Args...>, list_t>;

	public:
		clever_inout_ptr_t(Smart& s, Args... args)
		: core_t(s, std::forward_as_tuple(std::forward<Args>(args)...)) {
		}
	};

	template <typename Pointer,
		typename Smart,
		typename... Args>
	auto clever_inout_ptr(Smart& p, Args&&... args) noexcept {
		return clever_inout_ptr_t<Smart, Pointer, Args...>(p, std::forward<Args>(args)...);
	}

	template <typename Smart,
		typename... Args>
	auto clever_inout_ptr(Smart& p, Args&&... args) noexcept {
		using Pointer = meta::pointer_of_t<Smart>;
		return clever_inout_ptr<Pointer>(p, std::forward<Args>(args)...);
	}

} // namespace phd

#endif // PHD_OUT_PTR_CLEVER_INOUT_PTR_HPP
