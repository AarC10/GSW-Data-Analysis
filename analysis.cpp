#include <vector>
#include <filesystem>
#include <dirent.h>

using namespace std;
namespace plt = matplotlibcpp;


int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Only received " << argc - 1 << " arguments, but expected 3." << endl;
        cout << "Usage: " << argv[0] << " <start_time> <end_time> <directory>" <<  endl;
        return 1;
    }

    int startTime = stoi(argv[1]);
    int endTime = stoi(argv[2]);

    printf("%d\n", startTime);
    printf("%d\n", endTime);

    DIR *dir = opendir(argv[3]);
    struct dirent *ent = NULL;

    if (dir == NULL) {
        cout << "Could not open directory " << argv[3] << endl;
        return 1;
    }

    while ((ent = readdir(dir))) {
        printf("%s\n", ent->d_name);
    }




    return 0;
}