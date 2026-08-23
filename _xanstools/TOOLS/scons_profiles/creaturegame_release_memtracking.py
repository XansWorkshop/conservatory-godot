production = "yes"
dev_build = "no"
module_mono_enabled = "yes"
optimize = "speed_trace"
deprecated = "no"
allow_break_on_error = "no" # Custom parameter! See SConstruct in project root.
allow_release_memory_tracking = "yes" # Custom parameter! See SConstruct in project root.
website = "https://xansworkshop.com/godot"
d3d12 = "yes"
fast_unsafe = "no"
engine_update_check = "no"
use_mingw = "yes"
angle = "no"
precision = "single"

#### Some stuff for The Conservatory ####

# Custom netcode solution is implemented that better suits the game.
# In particular, most of The Conservatory is nodeless, and so built in replication does nothing.
# Additionally, a lot of the abstractions made by Godot do not align with personal design rules and
# so a custom solution is implemented to be significantly more strict with networking.
module_jsonrpc_enabled = "no"
module_multiplayer_enabled = "no"
module_enet_enabled = "no"
module_webrtc_enabled = "no"
module_websocket_enabled = "no"
module_mbedtls_enabled = "no"

# Probably the most radical change, this is due to code security. CasCore can enforce C# security but
# there is no way to lock down gdscript.
# EDIT: For CreatureGame, some of its plugins require gdscript to load. It's also not modding.
module_gdscript_enabled = "yes"

# Other things that are unused but which were left available for modders to possibly use.
'''
module_camera_enabled = "no"
module_csg_enabled = "no"
module_gridmap_enabled = "no"
module_msdfgen_enabled = "no"
'''

# Stuff for platforms that aren't supported.
module_mobile_vr_enabled = "no"
module_webxr_enabled = "no"
