/* Copyright 2023, Robert Bendun. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  If you would like to incorporate Link into a proprietary software application,
 *  please contact <link-devs@ableton.com>.
 */

#pragma once

#include <ableton/discovery/NetworkByteStreamSerializable.hpp>
#include <ableton/link/Beats.hpp>
#include <ableton/link/Tempo.hpp>
#include <cmath>
#include <cstdint>
#include <tuple>

namespace ableton
{
namespace link
{

struct Echo
{
  static const std::int32_t key = 'echo';
  static_assert(key == 0x6563686f, "Unexpected byte order");

  // Model the NetworkByteStreamSerializable concept
  friend std::uint32_t sizeInByteStream(const Echo& echo)
  {
		using namespace ableton::discovery;
    return sizeInByteStream(echo.bytes.size()) + echo.bytes.size();
  }

  template <typename It>
  friend It toNetworkByteStream(const Echo& echo, It out)
  {
		using namespace ableton::discovery;
		out = toNetworkByteStream(echo.bytes.size(), out);
		return std::copy(echo.bytes.begin(), echo.bytes.end(), out);
  }

  template <typename It>
  static std::pair<Echo, It> fromNetworkByteStream(It begin, It end)
  {
		Echo echo;
		decltype(echo.bytes.size()) size;
		std::tie(size, begin) = discovery::Deserialize<decltype(echo.bytes.size())>::fromNetworkByteStream(begin, end);
		auto const parsed_end = std::next(begin, size);
		echo.bytes.assign(begin, parsed_end);
		return { std::move(echo), parsed_end };
  }

	std::string bytes{};
};

} // namespace link
} // namespace ableton

