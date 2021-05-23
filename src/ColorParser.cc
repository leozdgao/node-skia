#include <iostream>

#include "ColorParser.h"

namespace node_skia_color {
    const SkColorMap SkColorParser::semantic_color_map = {
        // 'transparent' keyword
        { "transparent", SK_ColorTRANSPARENT },
        /**
         * @desc: basic color keywords * extended color keywords
         * @doc: https://www.w3.org/TR/2018/REC-css-color-3-20180619/#svg-color
         */
        { "aliceblue", SkColorSetARGB(0xFF, 0xF0, 0xF8, 0xFF) },
        { "antiquewhite", SkColorSetARGB(0xFF, 0xFA, 0xEB, 0xD7) },
        { "aqua", SkColorSetARGB(0xFF, 0x00, 0xFF, 0xFF) },
        { "aquamarine", SkColorSetARGB(0xFF, 0x7F, 0xFF, 0xD4) },
        { "azure", SkColorSetARGB(0xFF, 0xF0, 0xFF, 0xFF) },
        { "beige", SkColorSetARGB(0xFF, 0xF5, 0xF5, 0xDC) },
        { "bisque", SkColorSetARGB(0xFF, 0xFF, 0xE4, 0xC4) },
        { "black", SkColorSetARGB(0xFF, 0x00, 0x00, 0x00) },
        { "blanchedalmond", SkColorSetARGB(0xFF, 0xFF, 0xEB, 0xCD) },
        { "blue", SkColorSetARGB(0xFF, 0x00, 0x00, 0xFF) },
        { "blueviolet", SkColorSetARGB(0xFF, 0x8A, 0x2B, 0xE2) },
        { "brown", SkColorSetARGB(0xFF, 0xA5, 0x2A, 0x2A) },
        { "burlywood", SkColorSetARGB(0xFF, 0xDE, 0xB8, 0x87) },
        { "cadetblue", SkColorSetARGB(0xFF, 0x5F, 0x9E, 0xA0) },
        { "chartreuse", SkColorSetARGB(0xFF, 0x7F, 0xFF, 0x00) },
        { "chocolate", SkColorSetARGB(0xFF, 0xD2, 0x69, 0x1E) },
        { "coral", SkColorSetARGB(0xFF, 0xFF, 0x7F, 0x50) },
        { "cornflowerblue", SkColorSetARGB(0xFF, 0x64, 0x95, 0xED) },
        { "cornsilk", SkColorSetARGB(0xFF, 0xFF, 0xF8, 0xDC) },
        { "crimson", SkColorSetARGB(0xFF, 0xDC, 0x14, 0x3C) },
        { "cyan", SkColorSetARGB(0xFF, 0x00, 0xFF, 0xFF) },
        { "darkblue", SkColorSetARGB(0xFF, 0x00, 0x00, 0x8B) },
        { "darkcyan", SkColorSetARGB(0xFF, 0x00, 0x8B, 0x8B) },
        { "darkgoldenrod", SkColorSetARGB(0xFF, 0xB8, 0x86, 0x0B) },
        { "darkgray", SkColorSetARGB(0xFF, 0xA9, 0xA9, 0xA9) },
        { "darkgreen", SkColorSetARGB(0xFF, 0x00, 0x64, 0x00) },
        { "darkgrey", SkColorSetARGB(0xFF, 0xA9, 0xA9, 0xA9) },
        { "darkkhaki", SkColorSetARGB(0xFF, 0xBD, 0xB7, 0x6B) },
        { "darkmagenta", SkColorSetARGB(0xFF, 0x8B, 0x00, 0x8B) },
        { "darkolivegreen", SkColorSetARGB(0xFF, 0x55, 0x6B, 0x2F) },
        { "darkorange", SkColorSetARGB(0xFF, 0xFF, 0x8C, 0x00) },
        { "darkorchid", SkColorSetARGB(0xFF, 0x99, 0x32, 0xCC) },
        { "darkred", SkColorSetARGB(0xFF, 0x8B, 0x00, 0x00) },
        { "darksalmon", SkColorSetARGB(0xFF, 0xE9, 0x96, 0x7A) },
        { "darkseagreen", SkColorSetARGB(0xFF, 0x8F, 0xBC, 0x8F) },
        { "darkslateblue", SkColorSetARGB(0xFF, 0x48, 0x3D, 0x8B) },
        { "darkslategray", SkColorSetARGB(0xFF, 0x2F, 0x4F, 0x4F) },
        { "darkslategrey", SkColorSetARGB(0xFF, 0x2F, 0x4F, 0x4F) },
        { "darkturquoise", SkColorSetARGB(0xFF, 0x00, 0xCE, 0xD1) },
        { "darkviolet", SkColorSetARGB(0xFF, 0x94, 0x00, 0xD3) },
        { "deeppink", SkColorSetARGB(0xFF, 0xFF, 0x14, 0x93) },
        { "deepskyblue", SkColorSetARGB(0xFF, 0x00, 0xBF, 0xFF) },
        { "dimgray", SkColorSetARGB(0xFF, 0x69, 0x69, 0x69) },
        { "dimgrey", SkColorSetARGB(0xFF, 0x69, 0x69, 0x69) },
        { "dodgerblue", SkColorSetARGB(0xFF, 0x1E, 0x90, 0xFF) },
        { "firebrick", SkColorSetARGB(0xFF, 0xB2, 0x22, 0x22) },
        { "floralwhite", SkColorSetARGB(0xFF, 0xFF, 0xFA, 0xF0) },
        { "forestgreen", SkColorSetARGB(0xFF, 0x22, 0x8B, 0x22) },
        { "fuchsia", SkColorSetARGB(0xFF, 0xFF, 0x00, 0xFF) },
        { "gainsboro", SkColorSetARGB(0xFF, 0xDC, 0xDC, 0xDC) },
        { "ghostwhite", SkColorSetARGB(0xFF, 0xF8, 0xF8, 0xFF) },
        { "gold", SkColorSetARGB(0xFF, 0xFF, 0xD7, 0x00) },
        { "goldenrod", SkColorSetARGB(0xFF, 0xDA, 0xA5, 0x20) },
        { "gray", SkColorSetARGB(0xFF, 0x80, 0x80, 0x80) },
        { "green", SkColorSetARGB(0xFF, 0x00, 0x80, 0x00) },
        { "greenyellow", SkColorSetARGB(0xFF, 0xAD, 0xFF, 0x2F) },
        { "grey", SkColorSetARGB(0xFF, 0x80, 0x80, 0x80) },
        { "honeydew", SkColorSetARGB(0xFF, 0xF0, 0xFF, 0xF0) },
        { "hotpink", SkColorSetARGB(0xFF, 0xFF, 0x69, 0xB4) },
        { "indianred", SkColorSetARGB(0xFF, 0xCD, 0x5C, 0x5C) },
        { "indigo", SkColorSetARGB(0xFF, 0x4B, 0x00, 0x82) },
        { "ivory", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0xF0) },
        { "khaki", SkColorSetARGB(0xFF, 0xF0, 0xE6, 0x8C) },
        { "lavender", SkColorSetARGB(0xFF, 0xE6, 0xE6, 0xFA) },
        { "lavenderblush", SkColorSetARGB(0xFF, 0xFF, 0xF0, 0xF5) },
        { "lawngreen", SkColorSetARGB(0xFF, 0x7C, 0xFC, 0x00) },
        { "lemonchiffon", SkColorSetARGB(0xFF, 0xFF, 0xFA, 0xCD) },
        { "lightblue", SkColorSetARGB(0xFF, 0xAD, 0xD8, 0xE6) },
        { "lightcoral", SkColorSetARGB(0xFF, 0xF0, 0x80, 0x80) },
        { "lightcyan", SkColorSetARGB(0xFF, 0xE0, 0xFF, 0xFF) },
        { "lightgoldenrodyellow", SkColorSetARGB(0xFF, 0xFA, 0xFA, 0xD2) },
        { "lightgray", SkColorSetARGB(0xFF, 0xD3, 0xD3, 0xD3) },
        { "lightgreen", SkColorSetARGB(0xFF, 0x90, 0xEE, 0x90) },
        { "lightgrey", SkColorSetARGB(0xFF, 0xD3, 0xD3, 0xD3) },
        { "lightpink", SkColorSetARGB(0xFF, 0xFF, 0xB6, 0xC1) },
        { "lightsalmon", SkColorSetARGB(0xFF, 0xFF, 0xA0, 0x7A) },
        { "lightseagreen", SkColorSetARGB(0xFF, 0x20, 0xB2, 0xAA) },
        { "lightskyblue", SkColorSetARGB(0xFF, 0x87, 0xCE, 0xFA) },
        { "lightslategray", SkColorSetARGB(0xFF, 0x77, 0x88, 0x99) },
        { "lightslategrey", SkColorSetARGB(0xFF, 0x77, 0x88, 0x99) },
        { "lightsteelblue", SkColorSetARGB(0xFF, 0xB0, 0xC4, 0xDE) },
        { "lightyellow", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0xE0) },
        { "lime", SkColorSetARGB(0xFF, 0x00, 0xFF, 0x00) },
        { "limegreen", SkColorSetARGB(0xFF, 0x32, 0xCD, 0x32) },
        { "linen", SkColorSetARGB(0xFF, 0xFA, 0xF0, 0xE6) },
        { "magenta", SkColorSetARGB(0xFF, 0xFF, 0x00, 0xFF) },
        { "maroon", SkColorSetARGB(0xFF, 0x80, 0x00, 0x00) },
        { "mediumaquamarine", SkColorSetARGB(0xFF, 0x66, 0xCD, 0xAA) },
        { "mediumblue", SkColorSetARGB(0xFF, 0x00, 0x00, 0xCD) },
        { "mediumorchid", SkColorSetARGB(0xFF, 0xBA, 0x55, 0xD3) },
        { "mediumpurple", SkColorSetARGB(0xFF, 0x93, 0x70, 0xDB) },
        { "mediumseagreen", SkColorSetARGB(0xFF, 0x3C, 0xB3, 0x71) },
        { "mediumslateblue", SkColorSetARGB(0xFF, 0x7B, 0x68, 0xEE) },
        { "mediumspringgreen", SkColorSetARGB(0xFF, 0x00, 0xFA, 0x9A) },
        { "mediumturquoise", SkColorSetARGB(0xFF, 0x48, 0xD1, 0xCC) },
        { "mediumvioletred", SkColorSetARGB(0xFF, 0xC7, 0x15, 0x85) },
        { "midnightblue", SkColorSetARGB(0xFF, 0x19, 0x19, 0x70) },
        { "mintcream", SkColorSetARGB(0xFF, 0xF5, 0xFF, 0xFA) },
        { "mistyrose", SkColorSetARGB(0xFF, 0xFF, 0xE4, 0xE1) },
        { "moccasin", SkColorSetARGB(0xFF, 0xFF, 0xE4, 0xB5) },
        { "navajowhite", SkColorSetARGB(0xFF, 0xFF, 0xDE, 0xAD) },
        { "navy", SkColorSetARGB(0xFF, 0x00, 0x00, 0x80) },
        { "oldlace", SkColorSetARGB(0xFF, 0xFD, 0xF5, 0xE6) },
        { "olive", SkColorSetARGB(0xFF, 0x80, 0x80, 0x00) },
        { "olivedrab", SkColorSetARGB(0xFF, 0x6B, 0x8E, 0x23) },
        { "orange", SkColorSetARGB(0xFF, 0xFF, 0xA5, 0x00) },
        { "orangered", SkColorSetARGB(0xFF, 0xFF, 0x45, 0x00) },
        { "orchid", SkColorSetARGB(0xFF, 0xDA, 0x70, 0xD6) },
        { "palegoldenrod", SkColorSetARGB(0xFF, 0xEE, 0xE8, 0xAA) },
        { "palegreen", SkColorSetARGB(0xFF, 0x98, 0xFB, 0x98) },
        { "paleturquoise", SkColorSetARGB(0xFF, 0xAF, 0xEE, 0xEE) },
        { "palevioletred", SkColorSetARGB(0xFF, 0xDB, 0x70, 0x93) },
        { "papayawhip", SkColorSetARGB(0xFF, 0xFF, 0xEF, 0xD5) },
        { "peachpuff", SkColorSetARGB(0xFF, 0xFF, 0xDA, 0xB9) },
        { "peru", SkColorSetARGB(0xFF, 0xCD, 0x85, 0x3F) },
        { "pink", SkColorSetARGB(0xFF, 0xFF, 0xC0, 0xCB) },
        { "plum", SkColorSetARGB(0xFF, 0xDD, 0xA0, 0xDD) },
        { "powderblue", SkColorSetARGB(0xFF, 0xB0, 0xE0, 0xE6) },
        { "purple", SkColorSetARGB(0xFF, 0x80, 0x00, 0x80) },
        { "red", SkColorSetARGB(0xFF, 0xFF, 0x00, 0x00) },
        { "rosybrown", SkColorSetARGB(0xFF, 0xBC, 0x8F, 0x8F) },
        { "royalblue", SkColorSetARGB(0xFF, 0x41, 0x69, 0xE1) },
        { "saddlebrown", SkColorSetARGB(0xFF, 0x8B, 0x45, 0x13) },
        { "salmon", SkColorSetARGB(0xFF, 0xFA, 0x80, 0x72) },
        { "sandybrown", SkColorSetARGB(0xFF, 0xF4, 0xA4, 0x60) },
        { "seagreen", SkColorSetARGB(0xFF, 0x2E, 0x8B, 0x57) },
        { "seashell", SkColorSetARGB(0xFF, 0xFF, 0xF5, 0xEE) },
        { "sienna", SkColorSetARGB(0xFF, 0xA0, 0x52, 0x2D) },
        { "silver", SkColorSetARGB(0xFF, 0xC0, 0xC0, 0xC0) },
        { "skyblue", SkColorSetARGB(0xFF, 0x87, 0xCE, 0xEB) },
        { "slateblue", SkColorSetARGB(0xFF, 0x6A, 0x5A, 0xCD) },
        { "slategray", SkColorSetARGB(0xFF, 0x70, 0x80, 0x90) },
        { "slategrey", SkColorSetARGB(0xFF, 0x70, 0x80, 0x90) },
        { "snow", SkColorSetARGB(0xFF, 0xFF, 0xFA, 0xFA) },
        { "springgreen", SkColorSetARGB(0xFF, 0x00, 0xFF, 0x7F) },
        { "steelblue", SkColorSetARGB(0xFF, 0x46, 0x82, 0xB4) },
        { "tan", SkColorSetARGB(0xFF, 0xD2, 0xB4, 0x8C) },
        { "teal", SkColorSetARGB(0xFF, 0x00, 0x80, 0x80) },
        { "thistle", SkColorSetARGB(0xFF, 0xD8, 0xBF, 0xD8) },
        { "tomato", SkColorSetARGB(0xFF, 0xFF, 0x63, 0x47) },
        { "turquoise", SkColorSetARGB(0xFF, 0x40, 0xE0, 0xD0) },
        { "violet", SkColorSetARGB(0xFF, 0xEE, 0x82, 0xEE) },
        { "wheat", SkColorSetARGB(0xFF, 0xF5, 0xDE, 0xB3) },
        { "white", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0xFF) },
        { "whitesmoke", SkColorSetARGB(0xFF, 0xF5, 0xF5, 0xF5) },
        { "yellow", SkColorSetARGB(0xFF, 0xFF, 0xFF, 0x00) },
        { "yellowgreen", SkColorSetARGB(0xFF, 0x9A, 0xCD, 0x32) }
    };
    /**
     * parse color string from expression, support:
     * - #RRGGBB
     * - #RGB
     * - #RRGGBBAA
     * - #RGBA
     * - rgb(r, g, b)
     * - rgba(r, g, b, a)
     * - hsl(h, s, l)
     * - hsla(h, s, l, a)
     * - sementics name
     */
    SkColor4f SkColorParser::rgba_from_string(string &str) {
        if (str[0] == '#') {

        } else if (str.rfind("rgba", 0) == 0) {

        } else if (str.rfind("rgb", 0) == 0) {

        } else if (str.rfind("hsla", 0) == 0) {

        } else if (str.rfind("hsl", 0) == 0) {

        } else {
            auto iter = semantic_color_map.find(str);

            if (iter != semantic_color_map.end()) {
                return SkColor4f::FromColor(iter->second);
            }
        }

        return SkColor4f::FromColor(SK_ColorBLACK);
    };

    
    

    // SkColor4f rgba_from_hex_string(string str) {
    //     size_t len = str.size();

    //     switch (len) {
    //     case 8:
    //         return rgba_from_hex8_string(str);        
    //     default:
    //         break;
    //     }
    // }

    // SkColor4f rgba_from_string(string str) {
    //     if (str[0] == '#') {
    //         return rgba_from_hex_string(str.substr(1));
    //     }
    // }
}
