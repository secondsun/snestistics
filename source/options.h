#pragma once

// This file is generated by utilities/command-line-parsing.py

#include <string>
#include <vector>
#include <stdint.h>

struct Options {
	enum PredictEnum {
		PRD_NEVER,       // No prediction
		PRD_FUNCTIONS,   // Only predict within annotated functions
		PRD_EVERYWHERE,  // Predict as much as possible
	};

	std::string                  rom_file;
	uint32_t                     rom_size = 0;
	std::vector<std::string>     trace_files;
	uint32_t                     nmi_first = 0;
	uint32_t                     nmi_last = 0;
	std::string                  trace_log_out_file;
	std::string                  script_file;
	std::vector<std::string>     labels_files;
	std::string                  auto_labels_file;
	bool                         auto_annotate = false;
	std::string                  symbol_fma_out_file;
	std::string                  symbol_mesen_s_out_file;
	std::string                  rewind_out_file;
	std::string                  report_out_file;
	std::string                  asm_out_file;
	std::string                  asm_header_file;
	bool                         asm_print_pc = true;
	bool                         asm_print_bytes = true;
	bool                         asm_print_register_sizes = true;
	bool                         asm_print_db = true;
	bool                         asm_print_dp = true;
	bool                         asm_lower_case_op = true;
	bool                         asm_correct_wla = false;
	PredictEnum                  predict = PRD_FUNCTIONS;
};

void parse_options(const int argc, const char * const argv[], Options &options);
