bl_info = {
    "name": "New Object",
    "author": "Your Name Here",
    "version": (1, 0),
    "blender": (2, 80, 0),
    "location": "View3D > Add > Mesh > New Object",
    "description": "Adds a new Mesh Object",
    "warning": "",
    "doc_url": "",
    "category": "Add Mesh",
}

import bpy
import socket

def main(context):
    
    sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    start=bpy.context.scene.frame_start
    end=bpy.context.scene.frame_end
    bpy.context.scene.frame_current = bpy.context.scene.frame_start
   
    for x in range(start,end):
        
        bpy.context.scene.frame_set(x)
        
        current= bpy.context.scene.frame_current
        
        sock.sendto(bytes(str(current), 'utf8'),("127.0.0.1",7777))


class SimpleOperator(bpy.types.Operator):
    """Tooltip"""
    bl_idname = "object.simple_operator"
    bl_label = "Send frames"

    @classmethod
    def poll(cls, context):
        return context.active_object is not None

    def execute(self, context):
        main(context)
        return {'FINISHED'}

def menu_func(self, context):
    self.layout.operator(SimpleOperator.bl_idname, text=SimpleOperator.bl_label)

class LayoutDemoPanel(bpy.types.Panel):
    """Creates a Panel in the scene context of the properties editor"""
    bl_label = "Layout Demo"
    bl_idname = "SCENE_PT_layout"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "scene"

    def draw(self, context):
        layout = self.layout

        scene = context.scene

        # Big render button
        layout.label(text="Big Button:")
        row = layout.row()
        row.scale_y = 3.0
        row.operator("object.simple_operator")

def register():
    bpy.utils.register_class(LayoutDemoPanel)
    bpy.utils.register_class(SimpleOperator)
    bpy.types.VIEW3D_MT_object.append(menu_func)


def unregister():
    bpy.utils.unregister_class(LayoutDemoPanel)
    bpy.utils.unregister_class(SimpleOperator)
    bpy.types.VIEW3D_MT_object.remove(menu_func)


if __name__ == "__main__":
    register()
