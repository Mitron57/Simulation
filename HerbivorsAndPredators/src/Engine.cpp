namespace Solution {
    using namespace SimulationParameters;

#ifdef WIN32
    std::int32_t Engine::rows;
#else
    winsize Engine::window {};
#endif

    Engine::Engine() {
        readConfig();
#ifdef WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
#endif
        std::signal(SIGINT, signalHandler);
    }

    void Engine::run() {
        printConfig();
        life.onAwake();
        while (true) {
            ticks++;
            movement.onUpdate();
            if (!life.onUpdate()) {
                endGame();
                break;
            }
            const std::string placeholder(fieldSize * 2, '-');
            std::cout << " " << placeholder << std::endl;
            for (auto& line : field) {
                std::cout << "| ";
                for (const char elem : line) {
                    std::cout << elem << " ";
                }
                std::cout << "|" << std::endl;
            }
            std::cout << " " << placeholder << std::endl;
#ifndef WIN32
            const std::int32_t rows = window.ws_row;
#endif
            for (std::int32_t i {};
                 i < std::abs(static_cast<std::int32_t>(fieldSize + 3) - rows);
                 ++i) {
                std::cout << std::endl;
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(300ms);
        }
    }

    void Engine::readConfig() {
        std::ifstream config {"config"};
        fieldSize = 20;
        maxAge = 20;
        maxReproductiveAge = 12;
        minReproductiveAge = 7;
        countOfPredators = 20;
        countOfHerbivors = 20;
        countOfGrass = 200;
        grassRegeneration = 0.35;
        cataclysmChance = 0.001;
        deathChance = 0.01;
        birthChance = 0.5;
        years = 50;
        placeholder = '.';
        if (config.is_open()) {
            for (std::size_t i {1}; !config.eof(); ++i) {
                std::string field {};
                std::getline(config, field);
                if (!field.empty()) {
                    const std::size_t delim {field.find(": ")};
                    if (delim == std::string::npos) {
                        std::cerr << "Unexpected field in config, line "
                                      << i << " ignored: \"" << field << "\""
                                      << std::endl;
                        continue;
                    }
                    const std::string& param {field.substr(0, delim)};
                    const std::string& value {
                        field.substr(delim + 2, std::string::npos)
                    };
                    if (std::ranges::all_of(value, [](auto elem) {
                            return '0' <= elem && elem <= '9';
                        })) {
                        if (param == "FieldSize") {
                            fieldSize = std::stoi(value);
                        } else if (param == "MaxAge") {
                            maxAge = std::stoi(value);
                        } else if (param == "MinReproductiveAge") {
                            minReproductiveAge = std::stoi(value);
                        } else if (param == "MaxReproductiveAge") {
                            maxReproductiveAge = std::stoi(value);
                        } else if (param == "CountOfHerbivors") {
                            countOfHerbivors = std::stoi(value);
                        } else if (param == "CountOfPredators") {
                            countOfPredators = std::stoi(value);
                        } else if (param == "CountOfGrass") {
                            countOfGrass = std::stoi(value);
                        } else if (param == "Years") {
                            years = std::stoi(value);
                        } else if (param == "GrassRegeneration") {
                            const std::uint32_t regen = std::stoi(value);
                            if (regen > 100) {
                                std::cerr << "Incorrect value in line " << i
                                          << " ignored: \"" << field << "\", "
                                          << value << " must be less than 100."
                                          << std::endl;
                            } else {
                                grassRegeneration = regen / 100.0;
                            }
                        } else if (param == "Cataclysm") {
                            const std::uint32_t percent = std::stoi(value);
                            if (percent > 100) {
                                std::cerr << "Incorrect value in line " << i
                                          << " ignored: \"" << field << "\", "
                                          << value << " must be less than 100."
                                          << std::endl;
                            } else {
                                cataclysmChance = percent / 100.0;
                            }
                        } else if (param == "BirthChance") {
                            const std::uint32_t percent = std::stoi(value);
                            if (percent > 100) {
                                std::cerr << "Incorrect value in line " << i
                                          << " ignored: \"" << field << "\", "
                                          << value << " must be less than 100."
                                          << std::endl;
                            } else {
                                birthChance = percent / 100.0;
                            }
                        } else if (param == "DeathChance") {
                            const std::uint32_t percent = std::stoi(value);
                            if (percent > 100) {
                                std::cerr << "Incorrect value in line " << i
                                          << " ignored: \"" << field << "\", "
                                          << value << " must be less than 100."
                                          << std::endl;
                            } else {
                                deathChance = percent / 100.0;
                            }
                        } else {
                            std::cerr << "Unexpected field in config, line "
                                      << i << " ignored: \"" << field << "\""
                                      << std::endl;
                        }
                    } else {
                        std::cerr << "Cannot recognize symbol in config, line "
                                  << i << " ignored: \"" << field << "\""
                                  << std::endl;
                    }
                }
            }
        } else {
            std::cout
                << "Cannot open file for configuration, simulation will use "
                   "default values..."
                << std::endl;
        }
        for (std::uint32_t i {}; i < fieldSize; ++i) {
            field.emplace_back(fieldSize, placeholder);
        }
    }

    void Engine::signalHandler(std::int32_t) {
        std::cout << std::endl;
        endGame();
        std::exit(0);
    }

    void Engine::printConfig() {
        std::cout << "============Config============" << std::endl;
        std::cout << "\tFieldSize: " << fieldSize << std::endl;
        std::cout << "\tMaxAge: " << maxAge << std::endl;
        std::cout << "\tMinReproductiveAge: " << minReproductiveAge << std::endl;
        std::cout << "\tMaxReproductiveAge: " << maxReproductiveAge << std::endl;
        std::cout << "\tCountOfHerbivors: " << countOfHerbivors << std::endl;
        std::cout << "\tCountOfPredators: " << countOfPredators << std::endl;
        std::cout << "\tCountOfGrass: " << countOfGrass << std::endl;
        std::cout << "\tYears: " << years << std::endl;
        std::cout << "\tGrassRegeneration: " << grassRegeneration << std::endl;
        std::cout << "\tCataclysm: " << cataclysmChance << std::endl;
        std::cout << "\tBirthChance: " << birthChance << std::endl;
        std::cout << "\tDeathChance: " << deathChance << std::endl;
#ifndef WIN32
        const std::int32_t rows {window.ws_row};
#endif
        for (std::int32_t i {}; i < std::abs(rows - 17); ++i) {
            std::cout << std::endl;
        }
        std::cout << "Start simulation?" << std::endl;
        std::cout << "Press any button to continue or Q to quit..." << std::endl;
        std::string choice {};
        std::getline(std::cin, choice);
        if (choice == "Q" || choice == "q") {
            system("clear");
            std::exit(0);
        }
    }


    void Engine::endGame() {
        std::cout << "============SIMULATION STATISTICS============"
                  << std::endl;
        std::cout << "\tCount of dead herbivors: " << deadHerbivors
                  << std::endl;
        std::cout << "\tCount of dead predators: " << deadPredators
                  << std::endl;
        std::cout << "\tLast lived herbivors count: " << countOfHerbivors
                  << std::endl;
        std::cout << "\tLast lived predators count: " << countOfPredators
                  << std::endl;
        std::cout << "\tMonths:" << ticks << std::endl;
#ifndef WIN32
        const std::int32_t rows {window.ws_row};
#endif
        const std::string placeholder(fieldSize * 2, '-');
        std::cout << " " << placeholder << std::endl;
        for (auto& line : field) {
            std::cout << "| ";
            for (const char elem : line) {
                std::cout << elem << " ";
            }
            std::cout << "|" << std::endl;
        }
        std::cout << " " << placeholder << std::endl;
        for (std::int32_t i {}; i < std::abs(rows - static_cast<std::int32_t>(fieldSize + 10)); ++i) {
            std::cout << std::endl;
        }
    }

}  // namespace Solution