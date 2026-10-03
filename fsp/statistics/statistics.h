#ifndef STATISTICS_STATISTICS_H
#define STATISTICS_STATISTICS_H

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <atomic>

namespace fsp::sc
{
	// statistics: namespace reserved for runtime statistics collection.
	// No public API is defined yet; this header exists as a placeholder
	// for future instrumentation.
	
	class Statistics
	{
	public:
		using clock = std::chrono::steady_clock;

		static void start() noexcept;

		static void stop() noexcept;

		static void enable() noexcept;

		static bool enabled() noexcept;

		static void file_scanned() noexcept;
		static void file_matched() noexcept;
		static void file_skipped() noexcept;
		static void file_error() noexcept;

		static void directory_scanned() noexcept;

		static void line_matched() noexcept;

		[[nodiscard]]
		static std::uint64_t files_scanned() noexcept;

		[[nodiscard]]
		static std::uint64_t files_matched() noexcept;

		[[nodiscard]]
		static std::uint64_t files_skipped() noexcept;

		[[nodiscard]]
		static std::uint64_t files_with_errors() noexcept;

		[[nodiscard]]
		static std::uint64_t directories_scanned() noexcept;

		[[nodiscard]]
		static std::uint64_t matched_lines() noexcept;

		[[nodiscard]]
		static std::uint64_t bytes_processed() noexcept;

		[[nodiscard]]
		static std::chrono::milliseconds elapsed() noexcept;

	private:
		static std::atomic<std::uint64_t> files_scanned_;
		static std::atomic<std::uint64_t> files_matched_;
		static std::atomic<std::uint64_t> files_skipped_;
		static std::atomic<std::uint64_t> files_with_errors_;

		static std::atomic<std::uint64_t> directories_scanned_;

		static std::atomic<std::uint64_t> matched_lines_;

		static Statistics::clock::time_point start_time_;
		static Statistics::clock::time_point end_time_;

		static bool enabled_;
	};
}

#endif // STATISTICS_STATISTICS_H