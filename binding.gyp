{
    "targets": [
        {
            "target_name": "addon",
            "sources": [
                "src/binding.cpp",
            ],
            "include_dirs": [
                "<!(node -p \"require('node-addon-api').include_dir\")",
                "src"
            ],
            "defines": [
                "NAPI_DISABLE_CPP_EXCEPTIONS",
            ],
            "conditions": [
                [
                    "OS==\"mac\"",
                    {
                        "cflags+": [
                            "-fvisibility=hidden"
                        ],
                        "xcode_settings": {
                            "GCC_SYMBOLS_PRIVATE_EXTERN": "YES"
                        }
                    }
                ]
            ]
        }
    ]
}
