<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="18008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="SP9500" Type="Folder">
			<Item Name="Algorithm" Type="Folder">
				<Item Name="R1CDEF" Type="Folder">
					<Item Name="R1C_Att_To_Bit.vi" Type="VI" URL="../SP9500/Algorithm/R1CDEF/R1C_Att_To_Bit.vi"/>
				</Item>
				<Item Name="K7_Offset.vi" Type="VI" URL="../SP9500/Algorithm/K7_Offset.vi"/>
			</Item>
			<Item Name="Types" Type="Folder">
				<Item Name="Att(0-0.5-31.5).ctl" Type="VI" URL="../SP9500/Types/Att(0-0.5-31.5).ctl"/>
				<Item Name="LO.ctl" Type="VI" URL="../SP9500/Types/LO.ctl"/>
				<Item Name="RF.ctl" Type="VI" URL="../SP9500/Types/RF.ctl"/>
				<Item Name="RF_Ver.ctl" Type="VI" URL="../SP9500/Types/RF_Ver.ctl"/>
				<Item Name="RFU.ctl" Type="VI" URL="../SP9500/Types/RFU.ctl"/>
			</Item>
			<Item Name="IO_Mode_Set.vi" Type="VI" URL="../SP9500/IO_Mode_Set.vi"/>
			<Item Name="LED_Set.vi" Type="VI" URL="../SP9500/LED_Set.vi"/>
			<Item Name="MCP23S17_Get.vi" Type="VI" URL="../SP9500/MCP23S17_Get.vi"/>
			<Item Name="MCP23S17_Set.vi" Type="VI" URL="../SP9500/MCP23S17_Set.vi"/>
			<Item Name="R1C_GPIO_A.vi" Type="VI" URL="../SP9500/R1C_GPIO_A.vi"/>
			<Item Name="R1C_RX_Att1.vi" Type="VI" URL="../SP9500/R1C_RX_Att1.vi"/>
			<Item Name="R1C_RX_Att2.vi" Type="VI" URL="../SP9500/R1C_RX_Att2.vi"/>
			<Item Name="R1C_RX_Att3.vi" Type="VI" URL="../SP9500/R1C_RX_Att3.vi"/>
			<Item Name="R1C_TRX_Att.vi" Type="VI" URL="../SP9500/R1C_TRX_Att.vi"/>
			<Item Name="R1C_TX_Att0.vi" Type="VI" URL="../SP9500/R1C_TX_Att0.vi"/>
			<Item Name="R1C_TX_Att1.vi" Type="VI" URL="../SP9500/R1C_TX_Att1.vi"/>
			<Item Name="R1C_TX_Att2.vi" Type="VI" URL="../SP9500/R1C_TX_Att2.vi"/>
			<Item Name="R1C_TX_Att3.vi" Type="VI" URL="../SP9500/R1C_TX_Att3.vi"/>
			<Item Name="RF_Rsrc.vi" Type="VI" URL="../SP9500/RF_Rsrc.vi"/>
			<Item Name="RFU_Rsrc.vi" Type="VI" URL="../SP9500/RFU_Rsrc.vi"/>
			<Item Name="Rsrc.vi" Type="VI" URL="../SP9500/Rsrc.vi"/>
		</Item>
		<Item Name="SP9500X" Type="Folder">
			<Item Name="RF_Boot.vi" Type="VI" URL="../SP9500X/RF_Boot.vi"/>
			<Item Name="RF_IQCapture.vi" Type="VI" URL="../SP9500X/RF_IQCapture.vi"/>
			<Item Name="RF_SetBitPath.vi" Type="VI" URL="../SP9500X/RF_SetBitPath.vi"/>
			<Item Name="RF_SetConnector.vi" Type="VI" URL="../SP9500X/RF_SetConnector.vi"/>
			<Item Name="RF_SetIQCaptureParam.vi" Type="VI" URL="../SP9500X/RF_SetIQCaptureParam.vi"/>
		</Item>
		<Item Name="IO Mode.ctl" Type="VI" URL="../Common/Types/IO Mode.ctl"/>
		<Item Name="RF_SetIQCaptureBuffer.vi" Type="VI" URL="../SP9500X/RF_SetIQCaptureBuffer.vi"/>
		<Item Name="RF_SetIQCaptureStart.vi" Type="VI" URL="../SP9500X/RF_SetIQCaptureStart.vi"/>
		<Item Name="Test.vi" Type="VI" URL="../SP9500X/Test.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="8.6CompatibleGlobalVar.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/config.llb/8.6CompatibleGlobalVar.vi"/>
				<Item Name="Check if File or Folder Exists.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/libraryn.llb/Check if File or Folder Exists.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="NI_FileType.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/lvfile.llb/NI_FileType.lvlib"/>
				<Item Name="NI_LVConfig.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/config.llb/NI_LVConfig.lvlib"/>
				<Item Name="NI_PackedLibraryUtility.lvlib" Type="Library" URL="/&lt;vilib&gt;/Utility/LVLibp/NI_PackedLibraryUtility.lvlib"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="VISA Register Access Address Space.ctl" Type="VI" URL="/&lt;vilib&gt;/Instr/_visa.llb/VISA Register Access Address Space.ctl"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
			</Item>
			<Item Name="FPGA_OP.vi" Type="VI" URL="../Common/FPGA_OP.vi"/>
			<Item Name="FPGA_Read.vi" Type="VI" URL="../Common/FPGA_Read.vi"/>
			<Item Name="FPGA_Wait_Idle.vi" Type="VI" URL="../Common/FPGA_Wait_Idle.vi"/>
			<Item Name="FPGA_Write.vi" Type="VI" URL="../Common/FPGA_Write.vi"/>
			<Item Name="rd.dll" Type="Document" URL="../Common/Driver/rd.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
