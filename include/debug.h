#pragma once

void disassembleInstruction(const Chunk &chunk, size_t offset);
void disassembleChunk(const Chunk &chunk, const std::string &name);