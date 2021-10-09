from building import *
import rtconfig

cwd = GetCurrentDir()
path = [cwd]
src = []

path += [
    cwd + '/inc']

# Host stack
src += Split('''
    src/bsal.c
    ''')

if GetDepend(['PKG_BSAL_SRV_BAS']):
	path += [cwd + '/profiles/service/basl_bas']
	src += Split("""
		profiles/service/basl_bas/bsal_srv_bas.c
		""")
		
if GetDepend(['PKG_BSAL_SRV_BLUFI']):
	path += [cwd + '/profiles/service/bsal_blufi']
	src += Split("""
		profiles/service/bsal_blufi/bsal_srv_blufi.c
		""")

if GetDepend(['PKG_BSAL_SRV_NUS']):
	path += [cwd + '/profiles/service/bsal_uart']
	src += Split("""
		profiles/service/bsal_uart/bsal_srv_uart.c
		""")

if GetDepend(['PKG_BSAL_SRV_LBS']):
	path += [cwd + '/profiles/service/bsal_lbs']
	src += Split("""
		profiles/service/bsal_lbs/bsal_srv_lbs.c
		""")

if GetDepend(['PKG_BSAL_SRV_DIS']):
	path += [cwd + '/profiles/service/bsal_dis']
	src += Split("""
		profiles/service/bsal_dis/bsal_srv_dis.c
		""")

if GetDepend(['PKG_BSAL_SRV_HRS']):
	path += [cwd + '/profiles/service/bsal_hrs']
	src += Split("""
		profiles/service/bsal_hrs/bsal_srv_hrs.c
		""")
		
if GetDepend(['PKG_BSAL_SAMPLE_BAS_ONLY']):
	src += Split("""
		samples/ble_bas_only_app.c
		""")
elif GetDepend(['PKG_BSAL_SAMPLE_BAS_BLUFI_COMBINE']):
    src += Split("""
		samples/ble_bas_blufi_app.c
		""")
elif GetDepend(['PKG_BSAL_SAMPLE_NUS_ONLY']):
    src += Split("""
		samples/ble_nus_app.c
		""")
elif GetDepend(['PKG_BSAL_SAMPLE_LBS_ONLY']):
    src += Split("""
		samples/ble_lbs_app.c
		""")
elif GetDepend(['PKG_BSAL_SAMPLE_HRS_ONLY']):
    src += Split("""
		samples/ble_hrs_app.c
		""")

		
if GetDepend(['PKG_BSAL_NIMBLE_STACK']):
	path += [cwd + '/port/nimble']
	src += Split("""
		port/nimble/bsal_nimble.c
		port/nimble/bsal_osif.c
		""")
		
elif GetDepend(['PKG_BSAL_RTK_STACK']):
	path += [cwd + '/port/realtek']
	src += Split("""
		port/realtek/bsal_rtk.c
		port/realtek/bsal_osif.c
		""")
		
elif GetDepend(['PKG_BSAL_EMPTY_STACK']):
	path += [cwd + '/port/empty_stack']
	src += Split("""
		port/empty_stack/bsal_osal_empty.c
		port/empty_stack/bsal_stack_empty.c
		""")

if rtconfig.CROSS_TOOL == 'keil':
    LOCAL_CCFLAGS = ' --gnu'
    
group = DefineGroup('bsal', src, depend = ['PKG_USING_BSAL'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
