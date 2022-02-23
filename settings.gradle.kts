fun lib(name: String) {
    include(name)
    project(":${name}").projectDir = rootDir.resolve("libs").resolve(name)
}

lib("util")
lib("os")
lib("network")
lib("gui")