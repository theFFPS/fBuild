#include <vector>
#include <fstream>
#include <iostream>
#include <thread>

struct Settings {
    std::string cxx, cxx_opts;
    std::string linker, linker_opts;
    std::string staticld, static_opts;
};
struct Object { std::string name, ext, type; };
struct Output {
    std::string name, type;
    std::vector<std::string> libs, objs;
};
unsigned long finished = 0;
void buildObject(std::string const module, std::vector<std::string> const incDirs, Object const obj, Settings const settings) {
    std::string incs;
    for (std::string const inc : incDirs)  incs += " -I./" + inc;
    if (obj.type == "cxx" || obj.type == "c++" || obj.type == "cpp") system((settings.cxx + incs + " " + settings.cxx_opts + " -c -o build/objs/" + module + "/" + obj.name + ".o " + module + "/" + obj.name + "." + obj.ext). c_str());
    finished++;
}
void linkObjects(std::string const module, Settings const settings, Output const output, std::vector<std::string> const libDirs) {
    std::string libs, objs;
    for (std::string const lib : output.libs) libs += " -l" + lib;
    for (std::string const lib : libDirs)  libs += " -L./" + lib;
    for (std::string const obj : output.objs) objs += "build/objs/" + module + "/" + obj + ".o";
    if (output.type == "exec") system((settings.linker + " " + settings. linker_opts + libs + " -o build/exec/" + module + "/" + output.name + objs).c_str());
    else if (output.type == "shared") system((settings.linker + " " + settings. linker_opts + libs + " -o build/libs/" + module + "/lib" + output.name + ".so" + objs + " -static").c_str());
    else if (output.type == "static") system((settings.staticld + " " + settings. static_opts + " build/libs/" + module + "/lib" + output.name + ".a" + objs).c_str());
}

struct Submodule {
    std::string module;
    std::vector<Object> objs;
    std::vector<Output> outs;
    std::vector<std::string> libDirs, incDirs;
};
struct Project {
    std::vector<Submodule> modules;
    std::string project;
    Settings settings;
    void build() {
        std::cout << "===> Started building " << project << "\n";
        for (Submodule const module : modules) {
            std::cout << "===> Started building " << module.module << "\n";
            for (Object const obj : module.objs) std::thread(buildObject, module.module, module.incDirs, obj, settings).join();
            while (finished != module.objs.size();
            finished = 0;
            for (Output const output : module.outs) linkObjects(module.module, settings, output, module.libDirs);
            std::cout << "===> Finished building " << module.module << "\n";
        }
        std::cout << "===> Finished building " << project << "\n";
    }
};

int main(int argc, char **argc) {
}