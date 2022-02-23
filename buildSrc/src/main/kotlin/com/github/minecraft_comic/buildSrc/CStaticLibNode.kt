@file:Suppress("UnstableApiUsage")

package com.github.minecraft_comic.buildSrc

import java.io.File

import org.gradle.language.c.tasks.CCompile
import org.gradle.language.cpp.tasks.CppCompile
import org.gradle.nativeplatform.tasks.CreateStaticLibrary

class CStaticLibNode internal constructor(
    val name: String,
    val cCompileTask: CCompile,
    val cppCompileTask: CppCompile,
    val linkTask: CreateStaticLibrary,
    val outputFile: File
) {
}