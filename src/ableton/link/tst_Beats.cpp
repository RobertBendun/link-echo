/* Copyright 2016, Ableton AG, Berlin. All rights reserved.
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

#include <ableton/link/Beats.hpp>
#include <ableton/test/CatchWrapper.hpp>

namespace ableton
{
namespace link
{

TEST_CASE("Beats")
{
  SECTION("ConstructFromFloating")
  {
    const auto beats = Beats{0.5};
    CHECK(500000 == beats.microBeats());
    CHECK_THAT(0.5, Catch::Matchers::WithinAbs(beats.floating(), 1e-10));
  }

  SECTION("ConstructFromMicros")
  {
    const auto beats = Beats{INT64_C(100000)};
    CHECK(100000 == beats.microBeats());
    CHECK_THAT(0.1, Catch::Matchers::WithinAbs(beats.floating(), 1e-10));
  }

  SECTION("Negation")
  {
    const auto beat = Beats{1.};
    CHECK(beat > -beat);
    CHECK(beat == -(-beat));
    CHECK(-beat < Beats{0.});
  }

  SECTION("Addition")
  {
    const auto beat1 = Beats{0.5};
    const auto beat2 = Beats{INT64_C(200000)};
    const auto beat3 = Beats{0.1};
    CHECK(beat1 == beat2 + beat2 + beat3);
  }

  SECTION("Subtraction")
  {
    const auto beat1 = Beats{0.5};
    const auto beat2 = Beats{INT64_C(200000)};
    const auto beat3 = Beats{0.1};
    CHECK(beat3 == beat1 - beat2 - beat2);
  }

  SECTION("Modulo")
  {
    const auto beat1 = Beats{0.1};
    const auto beat2 = Beats{0.5};
    const auto beat3 = Beats{0.6};
    const auto beat4 = Beats{0.};
    CHECK(beat1 == beat3 % beat2);
    CHECK(beat4 == beat3 % beat4);
  }

  SECTION("SizeInByteStream")
  {
    Beats beats{0.5};
    CHECK(8 == sizeInByteStream(beats));
  }

  SECTION("RoundtripByteStreamEncoding")
  {
    Beats beats{0.5};
    std::vector<std::uint8_t> bytes(sizeInByteStream(beats));
    const auto end = toNetworkByteStream(beats, begin(bytes));
    const auto result = Beats::fromNetworkByteStream(begin(bytes), end);
    CHECK(beats == result.first);
  }
}

} // namespace link
} // namespace ableton
