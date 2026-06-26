#include <cstddef>
#include <ranges>
#include <iostream>
#include <set>
#include <stack>
#include <stdexcept>
#include <vector>
#include <string_view>
#include <string>
#include <fstream>

class Interpreter {
public:
    inline static const std::set<char> VALID_BF_CHARS {'<', '>', '+', '-', '.', ',', '[', ']'};

private:
    std::vector<unsigned char> m_tape {};
    std::size_t m_pointer {};

private:
    std::vector<std::size_t> build_jmp_table(std::string_view instructions) {
        std::vector<std::size_t> jump_table(instructions.size());
        std::stack<std::size_t> stk {};

        for (std::size_t i = 0; i < instructions.size(); i++) {
            if (instructions[i] == '[') {
                stk.push(i);
            } else if (instructions[i] == ']') {
                if (stk.empty()) throw std::logic_error("no closing bracket found");
                auto open_idx {stk.top()};
                stk.pop();
                jump_table[open_idx] = i;
                jump_table[i] = open_idx;

            }
        }
        if (!stk.empty()) throw std::logic_error("no closing bracket found at the end of the instruction set");
        return jump_table;
    }

public:
    Interpreter(std::size_t size = 10000) : m_tape(size, 0) {}
    
    void run(std::string_view source_code) {
        std::string instructions {
            source_code 
            | std::ranges::views::filter([](char c) {return VALID_BF_CHARS.contains(c);})
            | std::ranges::to<std::string>()
        };
        auto jmp_table {build_jmp_table(instructions)};
        for (std::size_t ip {}; ip < instructions.size(); ip++) {
            char instruction {instructions[ip]};
            switch (instruction) {
                case '<':
                    if (m_pointer == 0) {
                        throw std::out_of_range("cannot have negetive pointer");
                    }
                    m_pointer--;
                    break;
                case '>':
                    m_pointer++;
                    if (m_pointer >= m_tape.size()) {
                        throw std::out_of_range("cannot have bigger pointer than tape");
                    }
                    break;
                case '+':
                    m_tape[m_pointer]++;
                    break;
                case '-':
                    m_tape[m_pointer]--;
                    break;
                case '.':
                    std::cout << static_cast<unsigned char>(m_tape[m_pointer]);
                    break;
                case ',':
                     m_tape[m_pointer] = static_cast<unsigned char>(std::cin.get());
                    break;
                case '[':
                    if (m_tape[m_pointer] == 0) {
                        ip = jmp_table[ip];
                    }
                    break;
                case ']':
                    if (m_tape[m_pointer] != 0) {
                        ip = jmp_table[ip];
                    }
                    break;
            }
        }

    }
};

int main (int argc, char *argv[]) {
    Interpreter interpreter {};

    if (argc < 2) {
    // for(const char &c :interpreter.VALID_BF_CHARS) std::cout << c <<'\n';
    interpreter.run("++++++++++[>+++++++>++++++++++>+++<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.");
        return 0;
    }
    std::ifstream source_file(argv[1]);
    std::string code = "";
    std::string line;
    while (getline(source_file, line)) {
        code += line;
    }
    interpreter.run(code);

    source_file.close();
    return 0;
}
