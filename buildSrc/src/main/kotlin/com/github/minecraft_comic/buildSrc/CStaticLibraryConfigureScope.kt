package com.github.minecraft_comic.buildSrc

import org.gradle.api.Project
import org.gradle.kotlin.dsl.getValue
import org.gradle.kotlin.dsl.provideDelegate
import org.gradle.kotlin.dsl.setValue

abstract class CStaticLibraryConfigureScope(project: Project) {
    private val objects = project.objects

    var name: String by objects.property(String::class.java).convention(project.name)
}