@file:Suppress("UnstableApiUsage", "UNCHECKED_CAST")

package com.github.minecraft_comic.buildSrc

import javax.inject.Inject

import org.gradle.api.GradleException
import org.gradle.api.Project
import org.gradle.kotlin.dsl.create
import org.gradle.kotlin.dsl.get
import org.gradle.kotlin.dsl.getValue
import org.gradle.kotlin.dsl.register
import org.gradle.language.c.tasks.CCompile
import org.gradle.language.cpp.CppPlatform
import org.gradle.language.cpp.internal.DefaultCppPlatform
import org.gradle.language.cpp.tasks.CppCompile
import org.gradle.language.cpp.plugins.CppLibraryPlugin
import org.gradle.language.nativeplatform.internal.toolchains.ToolChainSelector
import org.gradle.nativeplatform.TargetMachineFactory
import org.gradle.nativeplatform.internal.DefaultTargetMachineFactory
import org.gradle.nativeplatform.platform.internal.DefaultNativePlatform
import org.gradle.nativeplatform.tasks.CreateStaticLibrary
import org.gradle.api.Plugin as GradlePlugin
import org.gradle.plugins.ide.idea.model.IdeaModel


class CStaticLibraryPlugin @Inject constructor(
//    private val componentFactory: NativeComponentFactory,
    private val toolChainSelector: ToolChainSelector,
    private val targetMachineFactory: TargetMachineFactory
) : GradlePlugin<Project> {
    override fun apply(target: Project) {
        target.pluginManager.apply("standard-tool-chains")
        target.pluginManager.apply("idea")

        val idea = target.extensions["idea"] as IdeaModel //(IdeaModel.() -> Any) -> Unit

        val extension = target.extensions.create<CStaticLibraryConfigureScope>("library", target)

        val name = extension.name
        synchronized(registeredLibs) {
            if (name in registeredLibs) {
                throw GradleException("lib with nme '${name}' already exists")
            }

            val tchSelector = toolChainSelector.select(
                CppPlatform::class.java, DefaultCppPlatform((targetMachineFactory as DefaultTargetMachineFactory).host())
            )

            val tch = tchSelector.toolChain
            val tp = DefaultNativePlatform("current")
            val objDir = target.buildDir.resolve("objects")


            val cSources = target.fileTree("src/main/c") includes "**/*.c"
            val cppSources = target.fileTree("src/main/cpp") includes "**/*.cpp"
            val publicHeaders = target.fileTree("src/main/public").includes("**/*.h", "**/*.hpp")
            val resourcesDir = target.projectDir.resolve("src/main/resources")

            @Suppress("KotlinConstantConditions") val ext = tp.operatingSystem.run {
                isWindows && return@run "lib"
                isLinux && return@run "a"
                throw GradleException("Unsupported platform")
            }
            val libFile = target.buildDir.resolve("${name}.$ext")

            val cCompileTask by target.tasks.register<CCompile>("compileC") {
                source(cSources)
                includes(publicHeaders)
                objectFileDir.set(objDir)

                toolChain.set(tch)
                targetPlatform.set(tp)
            }

            val cppCompileTask by target.tasks.register<CppCompile>("compileCpp") {
                source(cppSources)
                includes.from(publicHeaders, tch)
                objectFileDir.set(objDir)

                toolChain.set(tch)
                targetPlatform.set(tp)
            }

            val linkTask by target.tasks.register<CreateStaticLibrary>("linkC") {
                dependsOn(cCompileTask, cppCompileTask)
                source(cCompileTask.objectFileDir.asFileTree + cppCompileTask.objectFileDir.asFileTree)
                outputFile.set(libFile)

                toolChain.set(tch)
                targetPlatform.set(tp)
            }

            registeredLibs.add(
                CStaticLibNode(
                    name = name,
                    outputFile = libFile,
                    cCompileTask = cCompileTask,
                    cppCompileTask = cppCompileTask,
                    linkTask = linkTask
                )
            )
        }
    }

    companion object {
        @JvmField
        val registeredLibs = object : HashSet<CStaticLibNode>() {
            operator fun get(name: String): CStaticLibNode = this.forEach { lib ->
                if (lib.name == name) return@get lib
            }.let {
                throw GradleException("lib with name '$name' not found")
            }
        }
    }
}