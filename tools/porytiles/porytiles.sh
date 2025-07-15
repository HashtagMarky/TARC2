## Based on Mako's Shell Script

## Relative Paths
case $0 in
  /*) script_path=$0 ;;
  *) script_path=$(pwd)/$0 ;;
esac

script_dir=$(dirname "$script_path")
dir_project=$(dirname "$(dirname "$script_dir")")/
porytiles_dir="${dir_project}tools/porytiles/"

dir_compiled_primary="${dir_project}data/tilesets/primary/"
dir_compiled_secondary="${dir_project}data/tilesets/secondary/"
metatile_behaviors="${dir_project}include/constants/metatile_behaviors.h"
dir_raw_tilesets="${porytiles_dir}raw/"
dir_raw_tilesets_primary="${dir_raw_tilesets}primary/"
dir_raw_tilesets_secondary="${dir_raw_tilesets}secondary/"

## Misc.
lastcmd=""
doubleconfirm="1"

attribute_generation=""
mostrar_menu() {
    echo 
    echo "PORYTILES HELPER MENU"
    echo "What to do?"
    echo "1. Compile primary tileset"
    echo "2. Compile secondary tileset"
    echo "3. Decompile primary tileset" 
    echo "4. Decompile secondary tileset"
    echo "5. Repeat last command (avoid unless certain what this does)"
    echo "6. Toggle double confirmation while compiling"
    echo "7. Edit current paths"
    echo "8. Bulk recompile secondary tilesets"
    if [ -z "$attribute_generation" ]; then
        echo "9. Toggle -disable-attribute-generation (Currently: Enabled)"
    else
        echo "9. Toggle -disable-attribute-generation (Currently: Disabled)"
    fi
    echo "0. Exit"
    read -p "Choose an option: " opcao
}

tileset=""
tilesetsrc=""
tilesetsrc2=""
while(true); do 
    mostrar_menu
    case $opcao in
        1) 
            # COMPILE PRIMARY

            if [ "$tileset" != "" ]; then
                echo "Current folder of the resulting primary tileset: " $tileset
                echo "Current folder containing the metatile layers in ${dir_raw_tilesets_primary} " $tilesetsrc
                read -p "Accept this set up? (y/n) " use
                if [ "$use" = "n" ]; then
                    read -p "Folder of the resulting primary tileset in data/tilesets: " tileset
                    read -p "Folder containing the metatile layers in ${dir_raw_tilesets_primary}: "   tilesetsrc
                else 
                    if [ "$doubleconfirm" = "1" ]; then
                        read -p "Input 'y' to confirm the current setup. " confirm
                        if [ "$confirm" != "y" ]; then
                            break
                        fi
                    fi
                fi
            else 
                    read -p "Folder of the resulting primary tileset in data/tilesets: " tileset
                    read -p "Folder containing the metatile layers in ${dir_raw_tilesets_primary}: "   tilesetsrc
            fi

            # run porytiles
            porytiles compile-primary ${attribute_generation} -Wall -o \
            "${dir_compiled_primary}${tileset}" \
            "${dir_raw_tilesets_primary}${tilesetsrc}" \
            "${metatile_behaviors}"

            lastcmd="porytiles compile-primary ${attribute_generation} -Wall -o \"${dir_compiled_primary}${tileset}\" \"${dir_raw_tilesets_primary}${tilesetsrc}\" \"${metatile_behaviors}\""

            echo "Compiled primary tileset."

            # compile seasonal tilesets
            if [ -f "${dir_raw_tilesets_primary}${tilesetsrc}/seasonal.csv" ]; then
                python3 "${porytiles_dir}seasonal_pals.py" \
                "${dir_compiled_primary}${tileset}/palettes/" \
                "${dir_raw_tilesets_primary}${tilesetsrc}/seasonal.csv"
            fi
            ;;
        2)
            if [ "$tileset" != "" ]; then
                echo "Current folder of the resulting compiled tileset in data/tilesets: " $tileset
                echo "Current folder in ${dir_raw_tilesets_secondary} that contains the metatile layers: "    $tilesetsrc
                echo "Current folder in ${dir_raw_tilesets_primary} for the related primary tileset: "     $tilesetsrc2
                read -p "Accept this setup? (y/n) " use
                if [ "$use" = "n" ]; then
                    read -p "Folder of the resulting compiled tileset in data/tilesets: " tileset

                    read -p "Folder in ${dir_raw_tilesets_secondary} that contains the metatile layers: "     tilesetsrc

                    read -p "Folder in ${dir_raw_tilesets_primary} for the related primary tileset: (-1 to use cached option)"  tilesetsrc2

                    if [ "$tilesetsrc2" = "-1" ]; then
                        tilesetsrc2=$(cat "${dir_raw_tilesets_secondary}${tilesetsrc}/primarysrc.txt")
                    fi
                else 
                    if [ "$doubleconfirm" = "1" ]; then
                        read -p "Input 'y' to confirm the current setup. " confirm
                        if [ "$confirm" != "y" ]; then
                            break
                        fi
                    fi
                fi
            else
                    read -p "Folder of the resulting compiled tileset in data/tilesets: " tileset

                    read -p "Folder in ${dir_raw_tilesets_secondary} that contains the metatile layers: "     tilesetsrc

                    read -p "Folder in ${dir_raw_tilesets_primary} for the related primary tileset: (-1 to use cached src) "  tilesetsrc2

                    if [ "$tilesetsrc2" = "-1" ]; then
                        tilesetsrc2=$(cat "${dir_raw_tilesets_secondary}${tilesetsrc}/primarysrc.txt")
                    fi
            fi

            # run porytiles
            porytiles compile-secondary ${attribute_generation} -Wall -o \
            "${dir_compiled_secondary}${tileset}" \
            "${dir_raw_tilesets_secondary}${tilesetsrc}" \
            "${dir_raw_tilesets_primary}${tilesetsrc2}" \
            "${metatile_behaviors}"

            lastcmd="porytiles compile-secondary ${attribute_generation} -Wall -o \"${dir_compiled_secondary}${tileset}\" \"${dir_raw_tilesets_secondary}${tilesetsrc}\" \"${dir_raw_tilesets_primary}${tilesetsrc2}\" \"${metatile_behaviors}\""

            # compile seasonal tilesets
            if [ -f "${dir_raw_tilesets_secondary}${tilesetsrc}/seasonal.csv" ]; then
                python3 "${porytiles_dir}seasonal_pals.py" \
                "${dir_compiled_secondary}${tileset}/palettes/" \
                "${dir_raw_tilesets_secondary}${tilesetsrc}/seasonal.csv"
            fi

            # write ${tilesetsrc2} to a .txt file for use later
            echo $tilesetsrc2 > ${dir_raw_tilesets_secondary}${tilesetsrc}/primarysrc.txt

            echo "Compiled secondary tileset."
            ;;
        3) 
            # DECOMPILE PRIMARY
            read -p "Folder name for the resulting decompiled tileset in ${dir_raw_tilesets}: " destino
            read -p "Folder name for the primary tileset to be decompiled in ${dir_compiled_primary}: " tilesetsrc

            # run porytiles
            porytiles decompile-primary -o "${dir_raw_tilesets}${destino}" \
            "${dir_compiled_primary}${tilesetsrc}" \
            "${metatile_behaviors}"

            lastcmd="porytiles decompile-primary -o \"${dir_raw_tilesets}${destino}\" \"${dir_compiled_primary}${tilesetsrc}\" \"${metatile_behaviors}\""


            echo "Decompiled primary tileset."
            ;;
        4)
            # DECOMPILE SECONDARY
            read -p "Folder name for the resulting decompiled tileset in ${dir_raw_tilesets}: " destino
            read -p "Folder name for the secondary tileset to be decompiled in ${dir_compiled_secondary}: " tilesetsrc
            read -p "Folder name for the linked primary tileset in ${dir_compiled_primary}: " tilesetsrc2

            # run porytiles
            porytiles decompile-secondary -o "${dir_raw_tilesets}${destino}" \
            "${dir_compiled_secondary}${tilesetsrc}" \
            "${dir_compiled_primary}${tilesetsrc2}" \
            "${metatile_behaviors}"

            lastcmd="porytiles decompile-secondary -o \"${dir_raw_tilesets}${destino}\" \"${dir_compiled_secondary}${tilesetsrc}\" \"${dir_compiled_primary}${tilesetsrc2}\" \"${metatile_behaviors}\""

            echo "Decompiled secondary tileset."
            ;;
        5)
            # run the command stored in lastcmd
            # show what is in lastcmd and confirm if the user wants to run it
            echo "Last command: " $lastcmd
            read -p "Run this command? (y/n) " run
            if [ "$run" = "s" ]; then
                $lastcmd
            fi
            ;;
        6) 
            if [ "$doubleconfirm" = "1" ]; then
                doubleconfirm="0"
                echo "Double confirmation disabled."
            else
                doubleconfirm="1"
                echo "Double confirmation enabled."
            fi
            ;;
        7) 
            echo "Current paths:"
            echo "1) Raw tilesets: " $dir_raw_tilesets
            echo "2) Compiled primary: " $dir_compiled_primary
            echo "3) Compiled secondary: " $dir_compiled_secondary
            echo "4) Metatile behaviors: " $metatile_behaviors
            while (true); do
                read -p "Which path would you like to change? (1-4) " path
                case $path in
                    1)
                        read -p "New raw tilesets path: " dir_raw_tilesets
                        ;;
                    2)
                        read -p "New compiled primary path: " dir_compiled_primary
                        ;;
                    3)
                        read -p "New compiled secondary path: " dir_compiled_secondary
                        ;;
                    4)
                        read -p "New metatile behaviors path: " metatile_behaviors
                        ;;
                    *)
                        echo "Invalid option."
                        ;;
                esac
                read -p "Change another path? (y/n) " change
                if [ "$change" = "n" ]; then
                    break
                fi
            done
            ;;

        8) 
        # bulk recompile secondary tilesets
            dir_path="${dir_raw_tilesets_secondary}"

            for folder in "$dir_path"*/; do
                echo "$(basename "$folder")"

                tileset=$(basename "$folder")
                tilesetsrc="$folder"

                # check if ${dir_compiled_secondary}${tileset} doesn't exist, which means that names differ and it will mess up, so break
                if [ ! -d "${dir_compiled_secondary}${tileset}" ]; then
                    echo "The folder ${dir_compiled_secondary}${tileset} does not exist, even though the raw secondary tileset is named $(basename "$folder"). Make sure the folder names match to use this command."
                    break
                fi

                # get the primary tileset linked with the secondary tileset
                tilesetsrc2=""
                tilesetsrc2=$(cat "$folder/primarysrc.txt")

                if [ "$tilesetsrc2" = "" ]; then
                    echo "Couldn't find the 'primarysrc.txt' file. Make sure the 'primarysrc.txt' file is in the correct folder."
                    break
                fi

                tilesetsrc2="/primary/${tilesetsrc2}"

                porytiles compile-secondary -Wall -o "${dir_compiled_secondary}${tileset}" \
                ${attribute_generation} \
                "${tilesetsrc}" \
                "${dir_raw_tilesets}${tilesetsrc2}" \
                "${metatile_behaviors}"


                echo "Compiled $(basename "$folder")."
            done
            ;;
        9)
            if [ "$attribute_generation" = "" ]; then
                attribute_generation="--disable-attribute-generation"
                echo "Attribute generation is now disabled."
            else
                attribute_generation=""
                echo "Attribute generation is now enabled."
            fi
            ;;
        0)
            # exit from the script
            break
            ;;
        *)
            echo "Invalid option."
            ;;
    esac
done 