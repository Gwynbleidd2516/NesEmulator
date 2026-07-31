#ifndef REGISTERS
#define REGISTERS

#include "Accumulator.h"
#include "Index.h"
#include "StackPointer.h"
#include "Flags.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

struct Registers
{
    Accumulator A;
    Index X;
    Index Y;
    Flags flags;
    // StackPointer sp;
    uint8_t sp;
    uint8_t *pc;
};

namespace fmt
{
    template <>
    struct formatter<Registers>
    {
        // Presentation format: 'x' for hex, 'd' for decimal, etc. (optional)
        char presentation = 'x'; // default to hex

        // Parse the format specifier (e.g., "{:x}" or "{:d}")
        constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin())
        {
            auto it = ctx.begin(), end = ctx.end();
            if (it != end && (*it == 'x' || *it == 'd' || *it == 'X'))
            {
                presentation = *it++;
            }
            return it;
        }

        // Format the Registers object
        template <typename FormatContext>
        auto format(const Registers &r, FormatContext &ctx) const -> decltype(ctx.out())
        {
            // Use format_to to append directly to the output buffer
            return fmt::format_to(
                ctx.out(),
                "A={:x}, X={:x}, Y={:x}, flags={:b}, sp={:x}, pc={:x}",
                r.A.getValue(), r.X.getValue(), r.Y.getValue(), r.flags.raw, r.sp, *r.pc);
        }
    };
} // namespace fmt

#endif