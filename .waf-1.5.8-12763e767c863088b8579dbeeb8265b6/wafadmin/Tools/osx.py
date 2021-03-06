#! /usr/bin/env python
# encoding: utf-8

import os,shutil,sys,platform
import TaskGen,Task,Build,Options
from TaskGen import taskgen,feature,after,before
from Logs import error,debug
def set_macosx_deployment_target(self):
	if self.env['MACOSX_DEPLOYMENT_TARGET']:
		os.environ['MACOSX_DEPLOYMENT_TARGET']=self.env['MACOSX_DEPLOYMENT_TARGET']
	elif'MACOSX_DEPLOYMENT_TARGET'not in os.environ:
		if sys.platform=='darwin':
			os.environ['MACOSX_DEPLOYMENT_TARGET']='.'.join(platform.mac_ver()[0].split('.')[:2])
def apply_framework(self):
	for x in self.to_list(self.env['FRAMEWORKPATH']):
		frameworkpath_st='-F%s'
		self.env.append_unique('CXXFLAGS',frameworkpath_st%x)
		self.env.append_unique('CCFLAGS',frameworkpath_st%x)
		self.env.append_unique('LINKFLAGS',frameworkpath_st%x)
	for x in self.to_list(self.env['FRAMEWORK']):
		self.env.append_value('LINKFLAGS',['-framework',x])
def create_task_macapp(self):
	if'cprogram'in self.features and self.link_task:
		apptask=self.create_task('macapp',self.env)
		apptask.set_inputs(self.link_task.outputs)
		apptask.set_outputs(self.link_task.outputs[0].change_ext('.app'))
		self.apptask=apptask
def apply_link_osx(self):
	if self.env['MACAPP']or getattr(self,'mac_app',False):
		self.create_task_macapp()
		name=self.link_task.outputs[0].name
		if getattr(self,'vnum',None):
			name=name.replace('.dylib','.%s.dylib'%self.vnum)
		path=os.path.join(self.env['PREFIX'],'lib',name)
		self.env.append_value('LINKFLAGS','-install_name')
		self.env.append_value('LINKFLAGS',path)
def apply_bundle(self):
	if not('cshlib'in self.features or'shlib'in self.features):return
	if self.env['MACBUNDLE']or getattr(self,'mac_bundle',False):
		self.env['shlib_PATTERN']=self.env['macbundle_PATTERN']
		uselib=self.uselib=self.to_list(self.uselib)
		if not'MACBUNDLE'in uselib:uselib.append('MACBUNDLE')
def apply_bundle_remove_dynamiclib(self):
	if self.env['MACBUNDLE']or getattr(self,'mac_bundle',False):
		if not getattr(self,'vnum',None):
			try:
				self.env['LINKFLAGS'].remove('-dynamiclib')
			except ValueError:
				pass
app_dirs=['Contents',os.path.join('Contents','MacOS'),os.path.join('Contents','Resources')]
app_info='''
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist SYSTEM "file://localhost/System/Library/DTDs/PropertyList.dtd">
<plist version="0.9">
<dict>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleGetInfoString</key>
	<string>Created by Waf</string>
	<key>CFBundleSignature</key>
	<string>????</string>
	<key>NOTE</key>
	<string>THIS IS A GENERATED FILE, DO NOT MODIFY</string>
	<key>CFBundleExecutable</key>
	<string>%s</string>
</dict>
</plist>
'''
def app_build(task):
	global app_dirs
	env=task.env
	i=0
	for p in task.outputs:
		srcfile=p.srcpath(env)
		debug('osx: creating directories')
		try:
			os.mkdir(srcfile)
			[os.makedirs(os.path.join(srcfile,d))for d in app_dirs]
		except(OSError,IOError):
			pass
		srcprg=task.inputs[i].srcpath(env)
		dst=os.path.join(srcfile,'Contents','MacOS')
		debug('osx: copy %s to %s'%(srcprg,dst))
		shutil.copy(srcprg,dst)
		debug('osx: generate Info.plist')
		f=open(os.path.join(srcfile,"Contents","Info.plist"),"w")
		f.write(app_info%os.path.basename(srcprg))
		f.close()
		i+=1
	return 0
def install_shlib(task):
	nums=task.vnum.split('.')
	path=self.install_path
	libname=task.outputs[0].name
	name3=libname.replace('.dylib','.%s.dylib'%task.vnum)
	name2=libname.replace('.dylib','.%s.dylib'%nums[0])
	name1=libname
	filename=task.outputs[0].abspath(task.env)
	bld=task.outputs[0].__class__.bld
	bld.install_as(path+name3,filename,env=task.env)
	bld.symlink_as(path+name2,name3)
	bld.symlink_as(path+name1,name3)
def install_target_osx_cshlib(self):
	if not self.bld.is_install:return
	if getattr(self,'vnum','')and sys.platform!='win32':
		self.link_task.install=install_shlib
Task.task_type_from_func('macapp',vars=[],func=app_build,after="cxx_link cc_link ar_link_static")

feature('cc','cxx')(set_macosx_deployment_target)
before('apply_lib_vars')(set_macosx_deployment_target)
feature('cc','cxx')(apply_framework)
after('apply_lib_vars')(apply_framework)
taskgen(create_task_macapp)
after('apply_link')(apply_link_osx)
feature('cc','cxx')(apply_link_osx)
before('apply_link','apply_lib_vars')(apply_bundle)
feature('cc','cxx')(apply_bundle)
after('apply_link')(apply_bundle_remove_dynamiclib)
feature('cshlib')(apply_bundle_remove_dynamiclib)
feature('osx')(install_target_osx_cshlib)
after('install_target_cshlib')(install_target_osx_cshlib)
