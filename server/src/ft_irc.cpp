#include "Server.hpp"

int     main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " port pass\n";
        return (1);
    }

    Server server(argv[1], argv[2]);

    server.create();
    server.run();

    return (0);
}