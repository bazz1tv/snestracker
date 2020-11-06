#pragma once

#define COMPANY_NAME "Black Hole Studios"
#define APP_NAME "SNES Tracker"
#define APP_VER_MAJOR 0
#define APP_VER_MINOR 2
#define APP_VER_MICRO 2
// concats the 3 version values into a string
#define APP_VER_STRING(maj, min, mic) #maj "." #min "." #mic
// allows calling the above macro with macro arguments
#define APP_VER_STRING2(maj, min, mic) APP_VER_STRING(maj, min, mic)
// The final actual "public" macro for a version string
#define APP_VERSION APP_VER_STRING2(APP_VER_MAJOR, APP_VER_MINOR, APP_VER_MICRO)
// public macro for the app name and version string
#define APP_NAME_VERSION APP_NAME " " APP_VERSION

/* use HTTP so it can be easily forwarded in the future. Site will automatically
convert it to HTTPS */
#define WIKI_URL "https://wiki.snestracker.com"
#define PATREON_URL "http://patreon.snestracker.com"
#define MERCH_URL "http://shop.snestracker.com"
#define STREAM_URL "https://stream.snestracker.com"
#define SOUNDCLOUD_URL "https://soundcloud.snestracker.com"
#define TWITTER_URL "https://twitter.snestracker.com"
