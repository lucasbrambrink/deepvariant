# Description:
# C library for high-throughput sequencing data formats
#

licenses(["notice"])

exports_files(["LICENSE"])

package(
    default_visibility = ["//visibility:public"],
    features = [
        "-layering_check",
        "-parse_headers",
    ],
)

version = "1.18"

version_dir = "htslib_1_18"

include_htslib = "htslib/" + version_dir

# The hermetic version uses labels, the non-hermetic version uses flags.

cc_library(
    name = "htslib_deps",
    linkopts = [
        "-llzma",
        "-lbz2",
        "-lz",
        "-lm",
        "-lpthread",
        # The following libraries are not needed, because we disable the GCS,
        # http and S3 file handlers:
        # "-lcurl",
        # "-lcrypto",
        # "-lzlib",
    ],
)

extra_libs = []

includes = [
    include_htslib,
    ".",
]

htslib_srcs = [
    # This is made with a genrule.  See below.
    "config.h",
] + [
    "bcf_sr_sort.h",
    "header.h",
    "hts_time_funcs.h",
    "hfile_internal.h",
    "hts_internal.h",
    "sam_internal.h",
    "sam_mods.c",
    "textutils_internal.h",
    "thread_pool_internal.h",
    "config_vars.h",
    "version.h",
    "bcf_sr_sort.c",
    "bgzf.c",
    "errmod.c",
    "faidx.c",
    "header.c",
    "hfile.c",
    # Disabled hfile modes:
    # "hfile_gcs.c",
    # "hfile_libcurl.c",
    # "hfile_s3.c",
    # "hfile_s3_write.c",
    "hts.c",
    "hts_expr.c",
    "hts_os.c",
    "htslib/sam.h",
    "kfunc.c",
    "kstring.c",
    "md5.c",
    "multipart.c",
    "plugin.c",
    "probaln.c",
    "realn.c",
    "regidx.c",
    "region.c",
    "sam.c",
    "synced_bcf_reader.c",
    "tbx.c",
    "textutils.c",
    "thread_pool.c",
    "vcf.c",
    "vcf_sweep.c",
    "vcfutils.c",
] + [
    "cram/cram_codecs.c",
    "cram/cram_decode.c",
    "cram/cram_encode.c",
    "cram/cram_external.c",
    "cram/cram_index.c",
    "cram/cram_io.c",
    "cram/cram_stats.c",
    "cram/mFILE.c",
    "cram/open_trace_file.c",
    "cram/pooled_alloc.c",
    "cram/string_alloc.c",
    "cram/mFILE.h",
    "cram/misc.h",
    "cram/open_trace_file.h",
    "cram/os.h",
    "cram/string_alloc.h",
] + [
    "htscodecs/htscodecs/arith_dynamic.h",
    "htscodecs/htscodecs/c_range_coder.h",
    "htscodecs/htscodecs/c_simple_model.h",
    "htscodecs/htscodecs/fqzcomp_qual.h",
    "htscodecs/htscodecs/htscodecs_endian.h",
    "htscodecs/htscodecs/htscodecs.h",
    "htscodecs/htscodecs/pack.h",
    "htscodecs/htscodecs/permute.h",
    "htscodecs/htscodecs/pooled_alloc.h",
    "htscodecs/htscodecs/rANS_byte.h",
    "htscodecs/htscodecs/rANS_static4x16.h",
    "htscodecs/htscodecs/rANS_static.h",
    "htscodecs/htscodecs/rANS_static16_int.h",
    "htscodecs/htscodecs/rANS_static32x16pr.h",
    "htscodecs/htscodecs/rANS_word.h",
    "htscodecs/htscodecs/rle.h",
    "htscodecs/htscodecs/tokenise_name3.h",
    "htscodecs/htscodecs/utils.h",
    "htscodecs/htscodecs/utils.c",
    "htscodecs/htscodecs/varint2.h",
    "htscodecs/htscodecs/varint.h",
    "htscodecs/htscodecs/version.h",
    "htscodecs/htscodecs/arith_dynamic.c",
    "htscodecs/htscodecs/fqzcomp_qual.c",
    "htscodecs/htscodecs/htscodecs.c",
    "htscodecs/htscodecs/pack.c",
    "htscodecs/htscodecs/rANS_static4x16pr.c",
    "htscodecs/htscodecs/rANS_static.c",
    "htscodecs/htscodecs/rANS_static32x16pr.c",
    "htscodecs/htscodecs/rANS_static32x16pr_avx2.c",
    "htscodecs/htscodecs/rANS_static32x16pr_neon.c",
    "htscodecs/htscodecs/rANS_static32x16pr_avx512.c",
    "htscodecs/htscodecs/rANS_static32x16pr_sse4.c",
    "htscodecs/htscodecs/rle.c",
    "htscodecs/htscodecs/tokenise_name3.c",
]

# These need to be included in the right order, cf cram.h
textual_hdrs = [
    "cram/cram_samtools.h",
    "cram/cram_structs.h",
    "cram/cram_io.h",
    "cram/cram_encode.h",
    "cram/cram_decode.h",
    "cram/cram_stats.h",
    "cram/cram_codecs.h",
    "cram/cram_index.h",
    "vcf.c",
]

# These become the exported API, c.f. ../BUILD.
htslib_hdrs = glob(["htslib/*.h"])

cram_hdrs = [
    "cram/cram.h",
    "cram/pooled_alloc.h",
]

# Just the headers required for the hfile API.
filegroup(
    name = "hfile_hdrs",
    srcs = [
        "hfile_internal.h",
        "htslib/hfile.h",
        "htslib/hts_defs.h",
        "htslib/kroundup.h",
        "htslib/kstring.h",
        "textutils_internal.h",
    ],
)

cc_library(
    name = "hfile_api",
    hdrs = [":hfile_hdrs"],
    visibility = ["//visibility:public"],
)

copts = [
    "-Wno-implicit-function-declaration",  # cram_io.c
    "-Wno-unused-variable",  # cram_encode.c
    "-Wno-error",
]

# This is the exported hdrs api.
filegroup(
    name = "htslib_hdrs",
    srcs = htslib_hdrs,
)

filegroup(
    name = "cram_hdrs",
    srcs = cram_hdrs,
)

# Genrules in lieu of ./configure.  Minimum viable linux.
genrule(
    name = "config_h",
    outs = ["config.h"],
    cmd = """
        exec > "$@"
        echo '#define HAVE_DRAND48 1'
        echo '#define HAVE_FDATASYNC 1'
        echo '#define HAVE_FSYNC 1'
        echo '#define HAVE_GETPAGESIZE 1'
        echo '#define HAVE_GMTIME_R 1'
        echo '#define HAVE_INTTYPES_H 1'
        echo '#define HAVE_LIBBZ2 1'
        echo '#define HAVE_LIBLZMA 1'
        echo '#define HAVE_LIBZ 1'
        echo '#define HAVE_MEMORY_H 1'
        echo '#define HAVE_MMAP 1'
        echo '#define HAVE_POPCNT 1'
        echo '#define HAVE_SSE4_1 1'
        echo '#define HAVE_SSSE3 1'
        echo '#define HAVE_STDINT_H 1'
        echo '#define HAVE_STDLIB_H 1'
        echo '#define HAVE_STRING_H 1'
        echo '#define HAVE_STRINGS_H 1'
        echo '#define HAVE_SYS_PARAM_H 1'
        echo '#define HAVE_SYS_STAT_H 1'
        echo '#define HAVE_SYS_TYPES_H 1'
        echo '#define HAVE_UNISTD_H 1'
        echo '#define _XOPEN_SOURCE 600'
        echo '#define UBSAN 1'
        echo '#define PACKAGE_BUGREPORT "samtools-help@lists.sourceforge.net"'
        echo '#define PACKAGE_NAME "HTSlib"'
        echo '#define PACKAGE_STRING "HTSlib %s"'
        echo '#define PACKAGE_TARNAME "htslib"'
        echo '#define PACKAGE_URL "http://www.htslib.org/"'
        echo '#define PACKAGE_VERSION "%s"'
        echo '#define PLUGIN_EXT ".so"'
        echo '#define STDC_HEADERS 1'
    """ % (version, version),
)

genrule(
    name = "version",
    outs = ["version.h"],
    cmd = """echo '#define HTS_VERSION_TEXT "%s"' > "$@" """ % version,
)

# From htslib-1.13/Makefile
genrule(
    name = "config_vars",
    outs = ["config_vars.h"],
    cmd = """
        echo '#define HTS_CC "gcc"' > $@
        echo '#define HTS_CPPFLAGS ""' >> $@
        echo '#define HTS_CFLAGS "-g -Wall -O2 -fvisibility=hidden"' >> $@
        echo '#define HTS_LDFLAGS "-fvisibility=hidden"' >> $@
        echo '#define HTS_LIBS "-lz -lm -lbz2 -llzma -lcurl"' >> $@
    """,
)

cc_library(
    name = "htslib",
    srcs = htslib_srcs,
    hdrs = htslib_hdrs + cram_hdrs,
    copts = copts,
    includes = includes,
    linkopts = extra_libs,
    textual_hdrs = textual_hdrs,
    visibility = ["//visibility:public"],
    deps = [":htslib_deps"],
)

# Misc binaries that might be exported.

cc_binary(
    name = "bgzip",
    srcs = ["bgzip.c"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "htsfile",
    srcs = ["htsfile.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "tabix",
    srcs = ["tabix.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [
        ":htslib",
    ],
)

cc_binary(
    name = "sam",
    srcs = ["test/sam.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

#  Things for testing.

cc_binary(
    name = "fieldarith",
    srcs = [
        "test/fieldarith.c",
    ],
    copts = [
        "-w",
    ],
    includes = [
        include_htslib,
    ],
    linkopts = extra_libs,
    deps = [
        ":htslib",
    ],
)

cc_binary(
    name = "hfile",
    srcs = ["test/hfile.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_index",
    srcs = ["test/test_index.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_regidx",
    srcs = ["test/test-regidx.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_realn",
    srcs = ["test/test_realn.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_view",
    srcs = ["test/test_view.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_vcf_api",
    srcs = ["test/test-vcf-api.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_vcf_sweep",
    srcs = ["test/test-vcf-sweep.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

# This captures some source files too, but they are small,
# so keep things simple with bare "*"s.
filegroup(
    name = "test_data",
    srcs = glob(
        [
            "test/*",
            "test/*/*",
        ],
        exclude_directories = 0,
    ),
)

cc_binary(
    name = "hts_endian",
    srcs = ["test/hts_endian.c"],
    copts = ["-w"],
    includes = [include_htslib],
    deps = [":htslib"],
)

cc_binary(
    name = "test_bgzf",
    srcs = ["test/test_bgzf.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test/thrash_threads6",
    srcs = ["test/thrash_threads6.c"],
    copts = ["$(STACK_FRAME_UNLIMITED)"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test/thrash_threads5",
    srcs = ["test/thrash_threads5.c"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test/thrash_threads4",
    srcs = ["test/thrash_threads4.c"],
    copts = ["$(STACK_FRAME_UNLIMITED)"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test/thrash_threads3",
    srcs = ["test/thrash_threads3.c"],
    copts = ["$(STACK_FRAME_UNLIMITED)"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test/thrash_threads2",
    srcs = ["test/thrash_threads2.c"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test/thrash_threads1",
    srcs = ["test/thrash_threads1.c"],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_bcf_sr",
    srcs = ["test/test-bcf-sr.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_bcf_translate",
    srcs = ["test/test-bcf-translate.c"],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)

cc_binary(
    name = "test_bcf_set_variant_type",
    srcs = [
        "test/test-bcf-set-variant-type.c",
    ],
    copts = ["-w"],
    includes = [include_htslib],
    linkopts = extra_libs,
    deps = [":htslib"],
)
