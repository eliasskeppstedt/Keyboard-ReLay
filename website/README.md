# Website
I cant bother myself atm to learn webdev which led me to vibe code the site. I think it looks pretty slick tbh, and works as intend with the new keymap system described in the ToDo section.

Current logic for loading custom remaps is inside of codes.js:

The base codes, VK_BASE_MAP, which contains the scan codes used for all computers, but also the US ansi vk codes, this is because the scan codes are based on these and we can then just fetch the ANSi scan codes from this list.

Then we have the SC_* layout maps. This will be used to build our scLayout, where ansi codes will be fetched from the base map, then we just fetch the ISO resp JIS scan codes if we need them into the scLayout also.

Then we have the VK_LANG which is lang dependent vk codes, those are used to, overlayed on the scLayout, get us our working layout based on the settings.