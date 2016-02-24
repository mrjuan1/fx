import bpy
from struct import pack

path='/home/juan/Desktop/fx/data/'

objs=bpy.data.objects;

for obj in objs:
	bpy.data.scenes[0].objects.active=obj
	obj.select=True
	bpy.ops.object.mode_set(mode='EDIT')
	bpy.ops.mesh.quads_convert_to_tris()
	bpy.ops.object.mode_set(mode='OBJECT')
	f=open(path+obj.name+'.bin','wb')
	for poly in obj.data.polygons:
		for i in range(0,3):
			index=poly.vertices[i]
			f.write(pack('f',obj.data.vertices[index].co.x))
			f.write(pack('f',obj.data.vertices[index].co.y))
			f.write(pack('f',obj.data.vertices[index].co.z))
			index=poly.loop_indices[i]
			f.write(pack('f',obj.data.uv_layers['uv'].data[index].uv.x))
			f.write(pack('f',(1.0-obj.data.uv_layers['uv'].data[index].uv.y)))
	f.close()
	obj.select=False

#

