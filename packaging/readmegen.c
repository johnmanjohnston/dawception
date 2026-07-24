#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: ./packaging [version] [ windows | macos | linux ]\n");
        return 1;
    }

    if (argv[1][0] == 'v') {
        printf("Do not include 'v' in version string\n");
        return 1;
    }

    for (int i = 0; argv[2][i]; i++) {
        argv[2][i] = tolower(argv[2][i]);
    }

    FILE *fptr;
    fptr = fopen("README.txt", "w");
    fprintf(fptr, "DAWception v%s", argv[1]);
    fprintf(fptr, "\n\n[Installation]\nTo complete installation, move "
                  "DAWception.vst3 to your DAW's VST3 "
                  "plugin folder.\n");

    if (strcmp(argv[2], "linux") == 0) {
        printf("DAWception v%s on Linux\n", argv[1]);

        fprintf(fptr,
                "\nStandard VST3 path for Linux:\n\t~/.vst3/ (user)\n\t\tOR "
                "\n\t/usr/lib/vst3/ (global)");
    } else if (strcmp(argv[2], "windows") == 0) {
        printf("DAWception v%s on Windows\n", argv[1]);

        fprintf(fptr, "\nStandard VST3 path for Windows: "
                      "\n\tC:\\Program Files\\Common Files\\VST3\\");
    } else if (strcmp(argv[2], "macos") == 0) {
        printf("DAWception v%s on macOS\n", argv[1]);

        fprintf(fptr, "\nStandard VST3 path for macOS: "
                      "\n\t/Library/Audio/Plug-Ins/VST3/");
        fprintf(fptr, "\n\nOn macOS, DAWception won't immediately run in your "
                      "DAW (because "
                      "DAWception is not code signed because I am broke)."
                      "\nTo fix this, open the Terminal app and run "
                      "the following commands (adjust VST3 path if needed):\n\t"

                      "sudo xattr -dr com.apple.quarantine "
                      "/Library/Audio/Plug-Ins/VST3/DAWception.vst3"
                      "\n\t"
                      "sudo codesign --deep --force --sign - "
                      "/Library/Audio/Plug-Ins/VST3/DAWception.vst3");
    } else {
        printf("unknown platform\n");
        fclose(fptr);
        return 1;
    }

    fprintf(fptr,
            "\n\n[License + Source]\nDAWception is licensed under the AGPLv3. "
            "See LICENSE, or "
            "https://github.com/johnmanjohnston/dawception/blob/main/LICENSE"
            " for details.");
    fprintf(fptr, "\nDAWception is free and open-source, available at "
                  "https://github.com/johnmanjohnston/dawception/");

    fclose(fptr);
    printf("written to README.txt\n");

    return 0;
}
