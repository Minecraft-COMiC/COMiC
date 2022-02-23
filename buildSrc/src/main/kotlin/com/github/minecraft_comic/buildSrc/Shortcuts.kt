@file:Suppress("NOTHING_TO_INLINE")

package com.github.minecraft_comic.buildSrc

import org.gradle.api.file.FileTree

infix fun FileTree.includes(pattern: String) = this.matching { include(pattern) }
infix fun FileTree.excludes(pattern: String) = this.matching { exclude(pattern) }
fun FileTree.includes(vararg patterns: String) = this.matching { include(*patterns) }
fun FileTree.excludes(vararg patterns: String) = this.matching { exclude(*patterns) }