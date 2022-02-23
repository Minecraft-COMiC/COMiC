import com.github.minecraft_comic.buildSrc.CStaticLibraryPlugin


subprojects {
    this@subprojects.projectDir.parentFile.apply {
        if (name != "libs") return@subprojects
        if (parentFile != rootDir) return@subprojects
    }
    this@subprojects.apply<CStaticLibraryPlugin>()
}