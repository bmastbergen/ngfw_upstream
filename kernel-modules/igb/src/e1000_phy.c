/*******************************************************************************

  Intel(R) Gigabit Ethernet Linux driver
  Copyright(c) 2007-2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  Linux NICS <linux.nics@intel.com>
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/

#include "e1000_api.h"

static s32 e1000_wait_autoneg(struct e1000_hw *hw);
/* Cable length tables */
static const u16 e1000_m88_cable_length_table[] = {
	0, 50, 80, 110, 140, 140, E1000_CABLE_LENGTH_UNDEFINED };
#define M88E1000_CABLE_LENGTH_TABLE_SIZE \
		(sizeof(e1000_m88_cable_length_table) / \
		 sizeof(e1000_m88_cable_length_table[0]))

static const u16 e1000_igp_2_cable_length_table[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 3, 5, 8, 11, 13, 16, 18, 21, 0, 0, 0, 3,
	6, 10, 13, 16, 19, 23, 26, 29, 32, 35, 38, 41, 6, 10, 14, 18, 22,
	26, 30, 33, 37, 41, 44, 48, 51, 54, 58, 61, 21, 26, 31, 35, 40,
	44, 49, 53, 57, 61, 65, 68, 72, 75, 79, 82, 40, 45, 51, 56, 61,
	66, 70, 75, 79, 83, 87, 91, 94, 98, 101, 104, 60, 66, 72, 77, 82,
	87, 92, 96, 100, 104, 108, 111, 114, 117, 119, 121, 83, 89, 95,
	100, 105, 109, 113, 116, 119, 122, 124, 104, 109, 114, 118, 121,
	124};
#define IGP02E1000_CABLE_LENGTH_TABLE_SIZE \
		(sizeof(e1000_igp_2_cable_length_table) / \
		 sizeof(e1000_igp_2_cable_length_table[0]))

/**
 *  e1000_init_phy_ops_generic - Initialize PHY function pointers
 *  @hw: pointer to the HW structure
 *
 *  Setups up the function pointers to no-op functions
 **/
void e1000_init_phy_ops_generic(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	DEBUGFUNC("e1000_init_phy_ops_generic");

	/* Initialize function pointers */
	phy->ops.init_params = e1000_null_ops_generic;
	phy->ops.acquire = e1000_null_ops_generic;
	phy->ops.check_polarity = e1000_null_ops_generic;
	phy->ops.check_reset_block = e1000_null_ops_generic;
	phy->ops.commit = e1000_null_ops_generic;
	phy->ops.force_speed_duplex = e1000_null_ops_generic;
	phy->ops.get_cfg_done = e1000_null_ops_generic;
	phy->ops.get_cable_length = e1000_null_ops_generic;
	phy->ops.get_info = e1000_null_ops_generic;
	phy->ops.set_page = e1000_null_set_page;
	phy->ops.read_reg = e1000_null_read_reg;
	phy->ops.read_reg_locked = e1000_null_read_reg;
	phy->ops.read_reg_page = e1000_null_read_reg;
	phy->ops.release = e1000_null_phy_generic;
	phy->ops.reset = e1000_null_ops_generic;
	phy->ops.set_d0_lplu_state = e1000_null_lplu_state;
	phy->ops.set_d3_lplu_state = e1000_null_lplu_state;
	phy->ops.write_reg = e1000_null_write_reg;
	phy->ops.write_reg_locked = e1000_null_write_reg;
	phy->ops.write_reg_page = e1000_null_write_reg;
	phy->ops.power_up = e1000_null_phy_generic;
	phy->ops.power_down = e1000_null_phy_generic;
	phy->ops.read_i2c_byte = e1000_read_i2c_byte_null;
	phy->ops.write_i2c_byte = e1000_write_i2c_byte_null;
}

/**
 *  e1000_null_set_page - No-op function, return 0
 *  @hw: pointer to the HW structure
 **/
s32 e1000_null_set_page(struct e1000_hw E1000_UNUSEDARG *hw,
			u16 E1000_UNUSEDARG data)
{
	DEBUGFUNC("e1000_null_set_page");
	return E1000_SUCCESS;
}

/**
 *  e1000_null_read_reg - No-op function, return 0
 *  @hw: pointer to the HW structure
 **/
s32 e1000_null_read_reg(struct e1000_hw E1000_UNUSEDARG *hw,
			u32 E1000_UNUSEDARG offset, u16 E1000_UNUSEDARG *data)
{
	DEBUGFUNC("e1000_null_read_reg");
	return E1000_SUCCESS;
}

/**
 *  e1000_null_phy_generic - No-op function, return void
 *  @hw: pointer to the HW structure
 **/
void e1000_null_phy_generic(struct e1000_hw E1000_UNUSEDARG *hw)
{
	DEBUGFUNC("e1000_null_phy_generic");
	return;
}

/**
 *  e1000_null_lplu_state - No-op function, return 0
 *  @hw: pointer to the HW structure
 **/
s32 e1000_null_lplu_state(struct e1000_hw E1000_UNUSEDARG *hw,
			  bool E1000_UNUSEDARG active)
{
	DEBUGFUNC("e1000_null_lplu_state");
	return E1000_SUCCESS;
}

/**
 *  e1000_null_write_reg - No-op function, return 0
 *  @hw: pointer to the HW structure
 **/
s32 e1000_null_write_reg(struct e1000_hw E1000_UNUSEDARG *hw,
			 u32 E1000_UNUSEDARG offset, u16 E1000_UNUSEDARG data)
{
	DEBUGFUNC("e1000_null_write_reg");
	return E1000_SUCCESS;
}

/**
 *  e1000_read_i2c_byte_null - No-op function, return 0
 *  @hw: pointer to hardware structure
 *  @byte_offset: byte offset to write
 *  @dev_addr: device address
 *  @data: data value read
 *
 **/
s32 e1000_read_i2c_byte_null(struct e1000_hw E1000_UNUSEDARG *hw,
			     u8 E1000_UNUSEDARG byte_offset,
			     u8 E1000_UNUSEDARG dev_addr,
			     u8 E1000_UNUSEDARG *data)
{
	DEBUGFUNC("e1000_read_i2c_byte_null");
	return E1000_SUCCESS;
}

/**
 *  e1000_write_i2c_byte_null - No-op function, return 0
 *  @hw: pointer to hardware structure
 *  @byte_offset: byte offset to write
 *  @dev_addr: device address
 *  @data: data value to write
 *
 **/
s32 e1000_write_i2c_byte_null(struct e1000_hw E1000_UNUSEDARG *hw,
			      u8 E1000_UNUSEDARG byte_offset,
			      u8 E1000_UNUSEDARG dev_addr,
			      u8 E1000_UNUSEDARG data)
{
	DEBUGFUNC("e1000_write_i2c_byte_null");
	return E1000_SUCCESS;
}

/**
 *  e1000_check_reset_block_generic - Check if PHY reset is blocked
 *  @hw: pointer to the HW structure
 *
 *  Read the PHY management control register and check whether a PHY reset
 *  is blocked.  If a reset is not blocked return E1000_SUCCESS, otherwise
 *  return E1000_BLK_PHY_RESET (12).
 **/
s32 e1000_check_reset_block_generic(struct e1000_hw *hw)
{
	u32 manc;

	DEBUGFUNC("e1000_check_reset_block");

	manc = E1000_READ_REG(hw, E1000_MANC);

	return (manc & E1000_MANC_BLK_PHY_RST_ON_IDE) ?
	       E1000_BLK_PHY_RESET : E1000_SUCCESS;
}

/**
 *  e1000_get_phy_id - Retrieve the PHY ID and revision
 *  @hw: pointer to the HW structure
 *
 *  Reads the PHY registers and stores the PHY ID and possibly the PHY
 *  revision in the hardware structure.
 **/
s32 e1000_get_phy_id(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val = E1000_SUCCESS;
	u16 phy_id;

	DEBUGFUNC("e1000_get_phy_id");

	if (!phy->ops.read_reg)
		return E1000_SUCCESS;

	ret_val = phy->ops.read_reg(hw, PHY_ID1, &phy_id);
	if (ret_val)
		return ret_val;

	phy->id = (u32)(phy_id << 16);
	usec_delay(20);
	ret_val = phy->ops.read_reg(hw, PHY_ID2, &phy_id);
	if (ret_val)
		return ret_val;

	phy->id |= (u32)(phy_id & PHY_REVISION_MASK);
	phy->revision = (u32)(phy_id & ~PHY_REVISION_MASK);

	return E1000_SUCCESS;
}

/**
 *  e1000_phy_reset_dsp_generic - Reset PHY DSP
 *  @hw: pointer to the HW structure
 *
 *  Reset the digital signal processor.
 **/
s32 e1000_phy_reset_dsp_generic(struct e1000_hw *hw)
{
	s32 ret_val;

	DEBUGFUNC("e1000_phy_reset_dsp_generic");

	if (!hw->phy.ops.write_reg)
		return E1000_SUCCESS;

	ret_val = hw->phy.ops.write_reg(hw, M88E1000_PHY_GEN_CONTROL, 0xC1);
	if (ret_val)
		return ret_val;

	return hw->phy.ops.write_reg(hw, M88E1000_PHY_GEN_CONTROL, 0);
}

/**
 *  e1000_read_phy_reg_mdic - Read MDI control register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Reads the MDI control register in the PHY at offset and stores the
 *  information read to data.
 **/
s32 e1000_read_phy_reg_mdic(struct e1000_hw *hw, u32 offset, u16 *data)
{
	struct e1000_phy_info *phy = &hw->phy;
	u32 i, mdic = 0;

	DEBUGFUNC("e1000_read_phy_reg_mdic");

	if (offset > MAX_PHY_REG_ADDRESS) {
		DEBUGOUT1("PHY Address %d is out of range\n", offset);
		return -E1000_ERR_PARAM;
	}

	/* Set up Op-code, Phy Address, and register offset in the MDI
	 * Control register.  The MAC will take care of interfacing with the
	 * PHY to retrieve the desired data.
	 */
	mdic = ((offset << E1000_MDIC_REG_SHIFT) |
		(phy->addr << E1000_MDIC_PHY_SHIFT) |
		(E1000_MDIC_OP_READ));

	E1000_WRITE_REG(hw, E1000_MDIC, mdic);

	/* Poll the ready bit to see if the MDI read completed
	 * Increasing the time out as testing showed failures with
	 * the lower time out
	 */
	for (i = 0; i < (E1000_GEN_POLL_TIMEOUT * 3); i++) {
		usec_delay_irq(50);
		mdic = E1000_READ_REG(hw, E1000_MDIC);
		if (mdic & E1000_MDIC_READY)
			break;
	}
	if (!(mdic & E1000_MDIC_READY)) {
		DEBUGOUT("MDI Read did not complete\n");
		return -E1000_ERR_PHY;
	}
	if (mdic & E1000_MDIC_ERROR) {
		DEBUGOUT("MDI Error\n");
		return -E1000_ERR_PHY;
	}
	if (((mdic & E1000_MDIC_REG_MASK) >> E1000_MDIC_REG_SHIFT) != offset) {
		DEBUGOUT2("MDI Read offset error - requested %d, returned %d\n",
			  offset,
			  (mdic & E1000_MDIC_REG_MASK) >> E1000_MDIC_REG_SHIFT);
		return -E1000_ERR_PHY;
	}
	*data = (u16) mdic;

	return E1000_SUCCESS;
}

/**
 *  e1000_write_phy_reg_mdic - Write MDI control register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write to register at offset
 *
 *  Writes data to MDI control register in the PHY at offset.
 **/
s32 e1000_write_phy_reg_mdic(struct e1000_hw *hw, u32 offset, u16 data)
{
	struct e1000_phy_info *phy = &hw->phy;
	u32 i, mdic = 0;

	DEBUGFUNC("e1000_write_phy_reg_mdic");

	if (offset > MAX_PHY_REG_ADDRESS) {
		DEBUGOUT1("PHY Address %d is out of range\n", offset);
		return -E1000_ERR_PARAM;
	}

	/* Set up Op-code, Phy Address, and register offset in the MDI
	 * Control register.  The MAC will take care of interfacing with the
	 * PHY to retrieve the desired data.
	 */
	mdic = (((u32)data) |
		(offset << E1000_MDIC_REG_SHIFT) |
		(phy->addr << E1000_MDIC_PHY_SHIFT) |
		(E1000_MDIC_OP_WRITE));

	E1000_WRITE_REG(hw, E1000_MDIC, mdic);

	/* Poll the ready bit to see if the MDI read completed
	 * Increasing the time out as testing showed failures with
	 * the lower time out
	 */
	for (i = 0; i < (E1000_GEN_POLL_TIMEOUT * 3); i++) {
		usec_delay_irq(50);
		mdic = E1000_READ_REG(hw, E1000_MDIC);
		if (mdic & E1000_MDIC_READY)
			break;
	}
	if (!(mdic & E1000_MDIC_READY)) {
		DEBUGOUT("MDI Write did not complete\n");
		return -E1000_ERR_PHY;
	}
	if (mdic & E1000_MDIC_ERROR) {
		DEBUGOUT("MDI Error\n");
		return -E1000_ERR_PHY;
	}
	if (((mdic & E1000_MDIC_REG_MASK) >> E1000_MDIC_REG_SHIFT) != offset) {
		DEBUGOUT2("MDI Write offset error - requested %d, returned %d\n",
			  offset,
			  (mdic & E1000_MDIC_REG_MASK) >> E1000_MDIC_REG_SHIFT);
		return -E1000_ERR_PHY;
	}

	return E1000_SUCCESS;
}

/**
 *  e1000_read_phy_reg_i2c - Read PHY register using i2c
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Reads the PHY register at offset using the i2c interface and stores the
 *  retrieved information in data.
 **/
s32 e1000_read_phy_reg_i2c(struct e1000_hw *hw, u32 offset, u16 *data)
{
	struct e1000_phy_info *phy = &hw->phy;
	u32 i, i2ccmd = 0;

	DEBUGFUNC("e1000_read_phy_reg_i2c");

	/* Set up Op-code, Phy Address, and register address in the I2CCMD
	 * register.  The MAC will take care of interfacing with the
	 * PHY to retrieve the desired data.
	 */
	i2ccmd = ((offset << E1000_I2CCMD_REG_ADDR_SHIFT) |
		  (phy->addr << E1000_I2CCMD_PHY_ADDR_SHIFT) |
		  (E1000_I2CCMD_OPCODE_READ));

	E1000_WRITE_REG(hw, E1000_I2CCMD, i2ccmd);

	/* Poll the ready bit to see if the I2C read completed */
	for (i = 0; i < E1000_I2CCMD_PHY_TIMEOUT; i++) {
		usec_delay(50);
		i2ccmd = E1000_READ_REG(hw, E1000_I2CCMD);
		if (i2ccmd & E1000_I2CCMD_READY)
			break;
	}
	if (!(i2ccmd & E1000_I2CCMD_READY)) {
		DEBUGOUT("I2CCMD Read did not complete\n");
		return -E1000_ERR_PHY;
	}
	if (i2ccmd & E1000_I2CCMD_ERROR) {
		DEBUGOUT("I2CCMD Error bit set\n");
		return -E1000_ERR_PHY;
	}

	/* Need to byte-swap the 16-bit value. */
	*data = ((i2ccmd >> 8) & 0x00FF) | ((i2ccmd << 8) & 0xFF00);

	return E1000_SUCCESS;
}

/**
 *  e1000_write_phy_reg_i2c - Write PHY register using i2c
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Writes the data to PHY register at the offset using the i2c interface.
 **/
s32 e1000_write_phy_reg_i2c(struct e1000_hw *hw, u32 offset, u16 data)
{
	struct e1000_phy_info *phy = &hw->phy;
	u32 i, i2ccmd = 0;
	u16 phy_data_swapped;

	DEBUGFUNC("e1000_write_phy_reg_i2c");

	/* Prevent overwritting SFP I2C EEPROM which is at A0 address.*/
	if ((hw->phy.addr == 0) || (hw->phy.addr > 7)) {
		DEBUGOUT1("PHY I2C Address %d is out of range.\n",
			  hw->phy.addr);
		return -E1000_ERR_CONFIG;
	}

	/* Swap the data bytes for the I2C interface */
	phy_data_swapped = ((data >> 8) & 0x00FF) | ((data << 8) & 0xFF00);

	/* Set up Op-code, Phy Address, and register address in the I2CCMD
	 * register.  The MAC will take care of interfacing with the
	 * PHY to retrieve the desired data.
	 */
	i2ccmd = ((offset << E1000_I2CCMD_REG_ADDR_SHIFT) |
		  (phy->addr << E1000_I2CCMD_PHY_ADDR_SHIFT) |
		  E1000_I2CCMD_OPCODE_WRITE |
		  phy_data_swapped);

	E1000_WRITE_REG(hw, E1000_I2CCMD, i2ccmd);

	/* Poll the ready bit to see if the I2C read completed */
	for (i = 0; i < E1000_I2CCMD_PHY_TIMEOUT; i++) {
		usec_delay(50);
		i2ccmd = E1000_READ_REG(hw, E1000_I2CCMD);
		if (i2ccmd & E1000_I2CCMD_READY)
			break;
	}
	if (!(i2ccmd & E1000_I2CCMD_READY)) {
		DEBUGOUT("I2CCMD Write did not complete\n");
		return -E1000_ERR_PHY;
	}
	if (i2ccmd & E1000_I2CCMD_ERROR) {
		DEBUGOUT("I2CCMD Error bit set\n");
		return -E1000_ERR_PHY;
	}

	return E1000_SUCCESS;
}

/**
 *  e1000_read_sfp_data_byte - Reads SFP module data.
 *  @hw: pointer to the HW structure
 *  @offset: byte location offset to be read
 *  @data: read data buffer pointer
 *
 *  Reads one byte from SFP module data stored
 *  in SFP resided EEPROM memory or SFP diagnostic area.
 *  Function should be called with
 *  E1000_I2CCMD_SFP_DATA_ADDR(<byte offset>) for SFP module database access
 *  E1000_I2CCMD_SFP_DIAG_ADDR(<byte offset>) for SFP diagnostics parameters
 *  access
 **/
s32 e1000_read_sfp_data_byte(struct e1000_hw *hw, u16 offset, u8 *data)
{
	u32 i = 0;
	u32 i2ccmd = 0;
	u32 data_local = 0;

	DEBUGFUNC("e1000_read_sfp_data_byte");

	if (offset > E1000_I2CCMD_SFP_DIAG_ADDR(255)) {
		DEBUGOUT("I2CCMD command address exceeds upper limit\n");
		return -E1000_ERR_PHY;
	}

	/* Set up Op-code, EEPROM Address,in the I2CCMD
	 * register. The MAC will take care of interfacing with the
	 * EEPROM to retrieve the desired data.
	 */
	i2ccmd = ((offset << E1000_I2CCMD_REG_ADDR_SHIFT) |
		  E1000_I2CCMD_OPCODE_READ);

	E1000_WRITE_REG(hw, E1000_I2CCMD, i2ccmd);

	/* Poll the ready bit to see if the I2C read completed */
	for (i = 0; i < E1000_I2CCMD_PHY_TIMEOUT; i++) {
		usec_delay(50);
		data_local = E1000_READ_REG(hw, E1000_I2CCMD);
		if (data_local & E1000_I2CCMD_READY)
			break;
	}
	if (!(data_local & E1000_I2CCMD_READY)) {
		DEBUGOUT("I2CCMD Read did not complete\n");
		return -E1000_ERR_PHY;
	}
	if (data_local & E1000_I2CCMD_ERROR) {
		DEBUGOUT("I2CCMD Error bit set\n");
		return -E1000_ERR_PHY;
	}
	*data = (u8) data_local & 0xFF;

	return E1000_SUCCESS;
}

/**
 *  e1000_write_sfp_data_byte - Writes SFP module data.
 *  @hw: pointer to the HW structure
 *  @offset: byte location offset to write to
 *  @data: data to write
 *
 *  Writes one byte to SFP module data stored
 *  in SFP resided EEPROM memory or SFP diagnostic area.
 *  Function should be called with
 *  E1000_I2CCMD_SFP_DATA_ADDR(<byte offset>) for SFP module database access
 *  E1000_I2CCMD_SFP_DIAG_ADDR(<byte offset>) for SFP diagnostics parameters
 *  access
 **/
s32 e1000_write_sfp_data_byte(struct e1000_hw *hw, u16 offset, u8 data)
{
	u32 i = 0;
	u32 i2ccmd = 0;
	u32 data_local = 0;

	DEBUGFUNC("e1000_write_sfp_data_byte");

	if (offset > E1000_I2CCMD_SFP_DIAG_ADDR(255)) {
		DEBUGOUT("I2CCMD command address exceeds upper limit\n");
		return -E1000_ERR_PHY;
	}
	/* The programming interface is 16 bits wide
	 * so we need to read the whole word first
	 * then update appropriate byte lane and write
	 * the updated word back.
	 */
	/* Set up Op-code, EEPROM Address,in the I2CCMD
	 * register. The MAC will take care of interfacing
	 * with an EEPROM to write the data given.
	 */
	i2ccmd = ((offset << E1000_I2CCMD_REG_ADDR_SHIFT) |
		  E1000_I2CCMD_OPCODE_READ);
	/* Set a command to read single word */
	E1000_WRITE_REG(hw, E1000_I2CCMD, i2ccmd);
	for (i = 0; i < E1000_I2CCMD_PHY_TIMEOUT; i++) {
		usec_delay(50);
		/* Poll the ready bit to see if lastly
		 * launched I2C operation completed
		 */
		i2ccmd = E1000_READ_REG(hw, E1000_I2CCMD);
		if (i2ccmd & E1000_I2CCMD_READY) {
			/* Check if this is READ or WRITE phase */
			if ((i2ccmd & E1000_I2CCMD_OPCODE_READ) ==
			    E1000_I2CCMD_OPCODE_READ) {
				/* Write the selected byte
				 * lane and update whole word
				 */
				data_local = i2ccmd & 0xFF00;
				data_local |= data;
				i2ccmd = ((offset <<
					E1000_I2CCMD_REG_ADDR_SHIFT) |
					E1000_I2CCMD_OPCODE_WRITE | data_local);
				E1000_WRITE_REG(hw, E1000_I2CCMD, i2ccmd);
			} else {
				break;
			}
		}
	}
	if (!(i2ccmd & E1000_I2CCMD_READY)) {
		DEBUGOUT("I2CCMD Write did not complete\n");
		return -E1000_ERR_PHY;
	}
	if (i2ccmd & E1000_I2CCMD_ERROR) {
		DEBUGOUT("I2CCMD Error bit set\n");
		return -E1000_ERR_PHY;
	}
	return E1000_SUCCESS;
}

/**
 *  e1000_read_phy_reg_m88 - Read m88 PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Acquires semaphore, if necessary, then reads the PHY register at offset
 *  and storing the retrieved information in data.  Release any acquired
 *  semaphores before exiting.
 **/
s32 e1000_read_phy_reg_m88(struct e1000_hw *hw, u32 offset, u16 *data)
{
	s32 ret_val;

	DEBUGFUNC("e1000_read_phy_reg_m88");

	if (!hw->phy.ops.acquire)
		return E1000_SUCCESS;

	ret_val = hw->phy.ops.acquire(hw);
	if (ret_val)
		return ret_val;

	ret_val = e1000_read_phy_reg_mdic(hw, MAX_PHY_REG_ADDRESS & offset,
					  data);

	hw->phy.ops.release(hw);

	return ret_val;
}

/**
 *  e1000_write_phy_reg_m88 - Write m88 PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Acquires semaphore, if necessary, then writes the data to PHY register
 *  at the offset.  Release any acquired semaphores before exiting.
 **/
s32 e1000_write_phy_reg_m88(struct e1000_hw *hw, u32 offset, u16 data)
{
	s32 ret_val;

	DEBUGFUNC("e1000_write_phy_reg_m88");

	if (!hw->phy.ops.acquire)
		return E1000_SUCCESS;

	ret_val = hw->phy.ops.acquire(hw);
	if (ret_val)
		return ret_val;

	ret_val = e1000_write_phy_reg_mdic(hw, MAX_PHY_REG_ADDRESS & offset,
					   data);

	hw->phy.ops.release(hw);

	return ret_val;
}

/**
 *  e1000_set_page_igp - Set page as on IGP-like PHY(s)
 *  @hw: pointer to the HW structure
 *  @page: page to set (shifted left when necessary)
 *
 *  Sets PHY page required for PHY register access.  Assumes semaphore is
 *  already acquired.  Note, this function sets phy.addr to 1 so the caller
 *  must set it appropriately (if necessary) after this function returns.
 **/
s32 e1000_set_page_igp(struct e1000_hw *hw, u16 page)
{
	DEBUGFUNC("e1000_set_page_igp");

	DEBUGOUT1("Setting page 0x%x\n", page);

	hw->phy.addr = 1;

	return e1000_write_phy_reg_mdic(hw, IGP01E1000_PHY_PAGE_SELECT, page);
}

/**
 *  __e1000_read_phy_reg_igp - Read igp PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *  @locked: semaphore has already been acquired or not
 *
 *  Acquires semaphore, if necessary, then reads the PHY register at offset
 *  and stores the retrieved information in data.  Release any acquired
 *  semaphores before exiting.
 **/
static s32 __e1000_read_phy_reg_igp(struct e1000_hw *hw, u32 offset, u16 *data,
				    bool locked)
{
	s32 ret_val = E1000_SUCCESS;

	DEBUGFUNC("__e1000_read_phy_reg_igp");

	if (!locked) {
		if (!hw->phy.ops.acquire)
			return E1000_SUCCESS;

		ret_val = hw->phy.ops.acquire(hw);
		if (ret_val)
			return ret_val;
	}

	if (offset > MAX_PHY_MULTI_PAGE_REG)
		ret_val = e1000_write_phy_reg_mdic(hw,
						   IGP01E1000_PHY_PAGE_SELECT,
						   (u16)offset);
	if (!ret_val)
		ret_val = e1000_read_phy_reg_mdic(hw,
						  MAX_PHY_REG_ADDRESS & offset,
						  data);
	if (!locked)
		hw->phy.ops.release(hw);

	return ret_val;
}

/**
 *  e1000_read_phy_reg_igp - Read igp PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Acquires semaphore then reads the PHY register at offset and stores the
 *  retrieved information in data.
 *  Release the acquired semaphore before exiting.
 **/
s32 e1000_read_phy_reg_igp(struct e1000_hw *hw, u32 offset, u16 *data)
{
	return __e1000_read_phy_reg_igp(hw, offset, data, false);
}

/**
 *  e1000_read_phy_reg_igp_locked - Read igp PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Reads the PHY register at offset and stores the retrieved information
 *  in data.  Assumes semaphore already acquired.
 **/
s32 e1000_read_phy_reg_igp_locked(struct e1000_hw *hw, u32 offset, u16 *data)
{
	return __e1000_read_phy_reg_igp(hw, offset, data, true);
}

/**
 *  e1000_write_phy_reg_igp - Write igp PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *  @locked: semaphore has already been acquired or not
 *
 *  Acquires semaphore, if necessary, then writes the data to PHY register
 *  at the offset.  Release any acquired semaphores before exiting.
 **/
static s32 __e1000_write_phy_reg_igp(struct e1000_hw *hw, u32 offset, u16 data,
				     bool locked)
{
	s32 ret_val = E1000_SUCCESS;

	DEBUGFUNC("e1000_write_phy_reg_igp");

	if (!locked) {
		if (!hw->phy.ops.acquire)
			return E1000_SUCCESS;

		ret_val = hw->phy.ops.acquire(hw);
		if (ret_val)
			return ret_val;
	}

	if (offset > MAX_PHY_MULTI_PAGE_REG)
		ret_val = e1000_write_phy_reg_mdic(hw,
						   IGP01E1000_PHY_PAGE_SELECT,
						   (u16)offset);
	if (!ret_val)
		ret_val = e1000_write_phy_reg_mdic(hw, MAX_PHY_REG_ADDRESS &
						       offset,
						   data);
	if (!locked)
		hw->phy.ops.release(hw);

	return ret_val;
}

/**
 *  e1000_write_phy_reg_igp - Write igp PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Acquires semaphore then writes the data to PHY register
 *  at the offset.  Release any acquired semaphores before exiting.
 **/
s32 e1000_write_phy_reg_igp(struct e1000_hw *hw, u32 offset, u16 data)
{
	return __e1000_write_phy_reg_igp(hw, offset, data, false);
}

/**
 *  e1000_write_phy_reg_igp_locked - Write igp PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Writes the data to PHY register at the offset.
 *  Assumes semaphore already acquired.
 **/
s32 e1000_write_phy_reg_igp_locked(struct e1000_hw *hw, u32 offset, u16 data)
{
	return __e1000_write_phy_reg_igp(hw, offset, data, true);
}

/**
 *  __e1000_read_kmrn_reg - Read kumeran register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *  @locked: semaphore has already been acquired or not
 *
 *  Acquires semaphore, if necessary.  Then reads the PHY register at offset
 *  using the kumeran interface.  The information retrieved is stored in data.
 *  Release any acquired semaphores before exiting.
 **/
static s32 __e1000_read_kmrn_reg(struct e1000_hw *hw, u32 offset, u16 *data,
				 bool locked)
{
	u32 kmrnctrlsta;

	DEBUGFUNC("__e1000_read_kmrn_reg");

	if (!locked) {
		s32 ret_val = E1000_SUCCESS;

		if (!hw->phy.ops.acquire)
			return E1000_SUCCESS;

		ret_val = hw->phy.ops.acquire(hw);
		if (ret_val)
			return ret_val;
	}

	kmrnctrlsta = ((offset << E1000_KMRNCTRLSTA_OFFSET_SHIFT) &
		       E1000_KMRNCTRLSTA_OFFSET) | E1000_KMRNCTRLSTA_REN;
	E1000_WRITE_REG(hw, E1000_KMRNCTRLSTA, kmrnctrlsta);
	E1000_WRITE_FLUSH(hw);

	usec_delay(2);

	kmrnctrlsta = E1000_READ_REG(hw, E1000_KMRNCTRLSTA);
	*data = (u16)kmrnctrlsta;

	if (!locked)
		hw->phy.ops.release(hw);

	return E1000_SUCCESS;
}

/**
 *  e1000_read_kmrn_reg_generic -  Read kumeran register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Acquires semaphore then reads the PHY register at offset using the
 *  kumeran interface.  The information retrieved is stored in data.
 *  Release the acquired semaphore before exiting.
 **/
s32 e1000_read_kmrn_reg_generic(struct e1000_hw *hw, u32 offset, u16 *data)
{
	return __e1000_read_kmrn_reg(hw, offset, data, false);
}

/**
 *  e1000_read_kmrn_reg_locked -  Read kumeran register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to be read
 *  @data: pointer to the read data
 *
 *  Reads the PHY register at offset using the kumeran interface.  The
 *  information retrieved is stored in data.
 *  Assumes semaphore already acquired.
 **/
s32 e1000_read_kmrn_reg_locked(struct e1000_hw *hw, u32 offset, u16 *data)
{
	return __e1000_read_kmrn_reg(hw, offset, data, true);
}

/**
 *  __e1000_write_kmrn_reg - Write kumeran register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *  @locked: semaphore has already been acquired or not
 *
 *  Acquires semaphore, if necessary.  Then write the data to PHY register
 *  at the offset using the kumeran interface.  Release any acquired semaphores
 *  before exiting.
 **/
static s32 __e1000_write_kmrn_reg(struct e1000_hw *hw, u32 offset, u16 data,
				  bool locked)
{
	u32 kmrnctrlsta;

	DEBUGFUNC("e1000_write_kmrn_reg_generic");

	if (!locked) {
		s32 ret_val = E1000_SUCCESS;

		if (!hw->phy.ops.acquire)
			return E1000_SUCCESS;

		ret_val = hw->phy.ops.acquire(hw);
		if (ret_val)
			return ret_val;
	}

	kmrnctrlsta = ((offset << E1000_KMRNCTRLSTA_OFFSET_SHIFT) &
		       E1000_KMRNCTRLSTA_OFFSET) | data;
	E1000_WRITE_REG(hw, E1000_KMRNCTRLSTA, kmrnctrlsta);
	E1000_WRITE_FLUSH(hw);

	usec_delay(2);

	if (!locked)
		hw->phy.ops.release(hw);

	return E1000_SUCCESS;
}

/**
 *  e1000_write_kmrn_reg_generic -  Write kumeran register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Acquires semaphore then writes the data to the PHY register at the offset
 *  using the kumeran interface.  Release the acquired semaphore before exiting.
 **/
s32 e1000_write_kmrn_reg_generic(struct e1000_hw *hw, u32 offset, u16 data)
{
	return __e1000_write_kmrn_reg(hw, offset, data, false);
}

/**
 *  e1000_write_kmrn_reg_locked -  Write kumeran register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Write the data to PHY register at the offset using the kumeran interface.
 *  Assumes semaphore already acquired.
 **/
s32 e1000_write_kmrn_reg_locked(struct e1000_hw *hw, u32 offset, u16 data)
{
	return __e1000_write_kmrn_reg(hw, offset, data, true);
}

/**
 *  e1000_set_master_slave_mode - Setup PHY for Master/slave mode
 *  @hw: pointer to the HW structure
 *
 *  Sets up Master/slave mode
 **/
static s32 e1000_set_master_slave_mode(struct e1000_hw *hw)
{
	s32 ret_val;
	u16 phy_data;

	/* Resolve Master/Slave mode */
	ret_val = hw->phy.ops.read_reg(hw, PHY_1000T_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	/* load defaults for future use */
	hw->phy.original_ms_type = (phy_data & CR_1000T_MS_ENABLE) ?
				   ((phy_data & CR_1000T_MS_VALUE) ?
				    e1000_ms_force_master :
				    e1000_ms_force_slave) : e1000_ms_auto;

	switch (hw->phy.ms_type) {
	case e1000_ms_force_master:
		phy_data |= (CR_1000T_MS_ENABLE | CR_1000T_MS_VALUE);
		break;
	case e1000_ms_force_slave:
		phy_data |= CR_1000T_MS_ENABLE;
		phy_data &= ~(CR_1000T_MS_VALUE);
		break;
	case e1000_ms_auto:
		phy_data &= ~CR_1000T_MS_ENABLE;
		/* fall-through */
	default:
		break;
	}

	return hw->phy.ops.write_reg(hw, PHY_1000T_CTRL, phy_data);
}

/**
 *  e1000_copper_link_setup_82577 - Setup 82577 PHY for copper link
 *  @hw: pointer to the HW structure
 *
 *  Sets up Carrier-sense on Transmit and downshift values.
 **/
s32 e1000_copper_link_setup_82577(struct e1000_hw *hw)
{
	s32 ret_val;
	u16 phy_data;

	DEBUGFUNC("e1000_copper_link_setup_82577");

	if (hw->phy.reset_disable)
		return E1000_SUCCESS;

	if (hw->phy.type == e1000_phy_82580) {
		ret_val = hw->phy.ops.reset(hw);
		if (ret_val) {
			DEBUGOUT("Error resetting the PHY.\n");
			return ret_val;
		}
	}

	/* Enable CRS on Tx. This must be set for half-duplex operation. */
	ret_val = hw->phy.ops.read_reg(hw, I82577_CFG_REG, &phy_data);
	if (ret_val)
		return ret_val;

	phy_data |= I82577_CFG_ASSERT_CRS_ON_TX;

	/* Enable downshift */
	phy_data |= I82577_CFG_ENABLE_DOWNSHIFT;

	ret_val = hw->phy.ops.write_reg(hw, I82577_CFG_REG, phy_data);
	if (ret_val)
		return ret_val;

	/* Set MDI/MDIX mode */
	ret_val = hw->phy.ops.read_reg(hw, I82577_PHY_CTRL_2, &phy_data);
	if (ret_val)
		return ret_val;
	phy_data &= ~I82577_PHY_CTRL2_MDIX_CFG_MASK;
	/* Options:
	 *   0 - Auto (default)
	 *   1 - MDI mode
	 *   2 - MDI-X mode
	 */
	switch (hw->phy.mdix) {
	case 1:
		break;
	case 2:
		phy_data |= I82577_PHY_CTRL2_MANUAL_MDIX;
		break;
	case 0:
	default:
		phy_data |= I82577_PHY_CTRL2_AUTO_MDI_MDIX;
		break;
	}
	ret_val = hw->phy.ops.write_reg(hw, I82577_PHY_CTRL_2, phy_data);
	if (ret_val)
		return ret_val;

	return e1000_set_master_slave_mode(hw);
}

/**
 *  e1000_copper_link_setup_m88 - Setup m88 PHY's for copper link
 *  @hw: pointer to the HW structure
 *
 *  Sets up MDI/MDI-X and polarity for m88 PHY's.  If necessary, transmit clock
 *  and downshift values are set also.
 **/
s32 e1000_copper_link_setup_m88(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data;

	DEBUGFUNC("e1000_copper_link_setup_m88");

	if (phy->reset_disable)
		return E1000_SUCCESS;

	/* Enable CRS on Tx. This must be set for half-duplex operation. */
	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	phy_data |= M88E1000_PSCR_ASSERT_CRS_ON_TX;

	/* Options:
	 *   MDI/MDI-X = 0 (default)
	 *   0 - Auto for all speeds
	 *   1 - MDI mode
	 *   2 - MDI-X mode
	 *   3 - Auto for 1000Base-T only (MDI-X for 10/100Base-T modes)
	 */
	phy_data &= ~M88E1000_PSCR_AUTO_X_MODE;

	switch (phy->mdix) {
	case 1:
		phy_data |= M88E1000_PSCR_MDI_MANUAL_MODE;
		break;
	case 2:
		phy_data |= M88E1000_PSCR_MDIX_MANUAL_MODE;
		break;
	case 3:
		phy_data |= M88E1000_PSCR_AUTO_X_1000T;
		break;
	case 0:
	default:
		phy_data |= M88E1000_PSCR_AUTO_X_MODE;
		break;
	}

	/* Options:
	 *   disable_polarity_correction = 0 (default)
	 *       Automatic Correction for Reversed Cable Polarity
	 *   0 - Disabled
	 *   1 - Enabled
	 */
	phy_data &= ~M88E1000_PSCR_POLARITY_REVERSAL;
	if (phy->disable_polarity_correction)
		phy_data |= M88E1000_PSCR_POLARITY_REVERSAL;

	ret_val = phy->ops.write_reg(hw, M88E1000_PHY_SPEC_CTRL, phy_data);
	if (ret_val)
		return ret_val;

	if (phy->revision < E1000_REVISION_4) {
		/* Force TX_CLK in the Extended PHY Specific Control Register
		 * to 25MHz clock.
		 */
		ret_val = phy->ops.read_reg(hw, M88E1000_EXT_PHY_SPEC_CTRL,
					    &phy_data);
		if (ret_val)
			return ret_val;

		phy_data |= M88E1000_EPSCR_TX_CLK_25;

		if ((phy->revision == E1000_REVISION_2) &&
		    (phy->id == M88E1111_I_PHY_ID)) {
			/* 82573L PHY - set the downshift counter to 5x. */
			phy_data &= ~M88EC018_EPSCR_DOWNSHIFT_COUNTER_MASK;
			phy_data |= M88EC018_EPSCR_DOWNSHIFT_COUNTER_5X;
		} else {
			/* Configure Master and Slave downshift values */
			phy_data &= ~(M88E1000_EPSCR_MASTER_DOWNSHIFT_MASK |
				     M88E1000_EPSCR_SLAVE_DOWNSHIFT_MASK);
			phy_data |= (M88E1000_EPSCR_MASTER_DOWNSHIFT_1X |
				     M88E1000_EPSCR_SLAVE_DOWNSHIFT_1X);
		}
		ret_val = phy->ops.write_reg(hw, M88E1000_EXT_PHY_SPEC_CTRL,
					     phy_data);
		if (ret_val)
			return ret_val;
	}

	/* Commit the changes. */
	ret_val = phy->ops.commit(hw);
	if (ret_val) {
		DEBUGOUT("Error committing the PHY changes\n");
		return ret_val;
	}

	return E1000_SUCCESS;
}

/**
 *  e1000_copper_link_setup_m88_gen2 - Setup m88 PHY's for copper link
 *  @hw: pointer to the HW structure
 *
 *  Sets up MDI/MDI-X and polarity for i347-AT4, m88e1322 and m88e1112 PHY's.
 *  Also enables and sets the downshift parameters.
 **/
s32 e1000_copper_link_setup_m88_gen2(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data;

	DEBUGFUNC("e1000_copper_link_setup_m88_gen2");

	if (phy->reset_disable)
		return E1000_SUCCESS;

	/* Enable CRS on Tx. This must be set for half-duplex operation. */
	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	/* Options:
	 *   MDI/MDI-X = 0 (default)
	 *   0 - Auto for all speeds
	 *   1 - MDI mode
	 *   2 - MDI-X mode
	 *   3 - Auto for 1000Base-T only (MDI-X for 10/100Base-T modes)
	 */
	phy_data &= ~M88E1000_PSCR_AUTO_X_MODE;

	switch (phy->mdix) {
	case 1:
		phy_data |= M88E1000_PSCR_MDI_MANUAL_MODE;
		break;
	case 2:
		phy_data |= M88E1000_PSCR_MDIX_MANUAL_MODE;
		break;
	case 3:
		/* M88E1112 does not support this mode) */
		if (phy->id != M88E1112_E_PHY_ID) {
			phy_data |= M88E1000_PSCR_AUTO_X_1000T;
			break;
		}
	case 0:
	default:
		phy_data |= M88E1000_PSCR_AUTO_X_MODE;
		break;
	}

	/* Options:
	 *   disable_polarity_correction = 0 (default)
	 *       Automatic Correction for Reversed Cable Polarity
	 *   0 - Disabled
	 *   1 - Enabled
	 */
	phy_data &= ~M88E1000_PSCR_POLARITY_REVERSAL;
	if (phy->disable_polarity_correction)
		phy_data |= M88E1000_PSCR_POLARITY_REVERSAL;

	/* Enable downshift and setting it to X6 */
	if (phy->id == M88E1543_E_PHY_ID) {
		phy_data &= ~I347AT4_PSCR_DOWNSHIFT_ENABLE;
		ret_val =
		    phy->ops.write_reg(hw, M88E1000_PHY_SPEC_CTRL, phy_data);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.commit(hw);
		if (ret_val) {
			DEBUGOUT("Error committing the PHY changes\n");
			return ret_val;
		}
	}

	phy_data &= ~I347AT4_PSCR_DOWNSHIFT_MASK;
	phy_data |= I347AT4_PSCR_DOWNSHIFT_6X;
	phy_data |= I347AT4_PSCR_DOWNSHIFT_ENABLE;

	ret_val = phy->ops.write_reg(hw, M88E1000_PHY_SPEC_CTRL, phy_data);
	if (ret_val)
		return ret_val;

	/* Commit the changes. */
	ret_val = phy->ops.commit(hw);
	if (ret_val) {
		DEBUGOUT("Error committing the PHY changes\n");
		return ret_val;
	}

	ret_val = e1000_set_master_slave_mode(hw);
	if (ret_val)
		return ret_val;

	return E1000_SUCCESS;
}

/**
 *  e1000_copper_link_setup_igp - Setup igp PHY's for copper link
 *  @hw: pointer to the HW structure
 *
 *  Sets up LPLU, MDI/MDI-X, polarity, Smartspeed and Master/Slave config for
 *  igp PHY's.
 **/
s32 e1000_copper_link_setup_igp(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;

	DEBUGFUNC("e1000_copper_link_setup_igp");

	if (phy->reset_disable)
		return E1000_SUCCESS;

	ret_val = hw->phy.ops.reset(hw);
	if (ret_val) {
		DEBUGOUT("Error resetting the PHY.\n");
		return ret_val;
	}

	/* Wait 100ms for MAC to configure PHY from NVM settings, to avoid
	 * timeout issues when LFS is enabled.
	 */
	msec_delay(100);

	/* disable lplu d0 during driver init */
	if (hw->phy.ops.set_d0_lplu_state) {
		ret_val = hw->phy.ops.set_d0_lplu_state(hw, false);
		if (ret_val) {
			DEBUGOUT("Error Disabling LPLU D0\n");
			return ret_val;
		}
	}
	/* Configure mdi-mdix settings */
	ret_val = phy->ops.read_reg(hw, IGP01E1000_PHY_PORT_CTRL, &data);
	if (ret_val)
		return ret_val;

	data &= ~IGP01E1000_PSCR_AUTO_MDIX;

	switch (phy->mdix) {
	case 1:
		data &= ~IGP01E1000_PSCR_FORCE_MDI_MDIX;
		break;
	case 2:
		data |= IGP01E1000_PSCR_FORCE_MDI_MDIX;
		break;
	case 0:
	default:
		data |= IGP01E1000_PSCR_AUTO_MDIX;
		break;
	}
	ret_val = phy->ops.write_reg(hw, IGP01E1000_PHY_PORT_CTRL, data);
	if (ret_val)
		return ret_val;

	/* set auto-master slave resolution settings */
	if (hw->mac.autoneg) {
		/* when autonegotiation advertisement is only 1000Mbps then we
		 * should disable SmartSpeed and enable Auto MasterSlave
		 * resolution as hardware default.
		 */
		if (phy->autoneg_advertised == ADVERTISE_1000_FULL) {
			/* Disable SmartSpeed */
			ret_val = phy->ops.read_reg(hw,
						    IGP01E1000_PHY_PORT_CONFIG,
						    &data);
			if (ret_val)
				return ret_val;

			data &= ~IGP01E1000_PSCFR_SMART_SPEED;
			ret_val = phy->ops.write_reg(hw,
						     IGP01E1000_PHY_PORT_CONFIG,
						     data);
			if (ret_val)
				return ret_val;

			/* Set auto Master/Slave resolution process */
			ret_val = phy->ops.read_reg(hw, PHY_1000T_CTRL, &data);
			if (ret_val)
				return ret_val;

			data &= ~CR_1000T_MS_ENABLE;
			ret_val = phy->ops.write_reg(hw, PHY_1000T_CTRL, data);
			if (ret_val)
				return ret_val;
		}

		ret_val = e1000_set_master_slave_mode(hw);
	}

	return ret_val;
}

/**
 *  e1000_phy_setup_autoneg - Configure PHY for auto-negotiation
 *  @hw: pointer to the HW structure
 *
 *  Reads the MII auto-neg advertisement register and/or the 1000T control
 *  register and if the PHY is already setup for auto-negotiation, then
 *  return successful.  Otherwise, setup advertisement and flow control to
 *  the appropriate values for the wanted auto-negotiation.
 **/
static s32 e1000_phy_setup_autoneg(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 mii_autoneg_adv_reg;
	u16 mii_1000t_ctrl_reg = 0;

	DEBUGFUNC("e1000_phy_setup_autoneg");

	phy->autoneg_advertised &= phy->autoneg_mask;

	/* Read the MII Auto-Neg Advertisement Register (Address 4). */
	ret_val = phy->ops.read_reg(hw, PHY_AUTONEG_ADV, &mii_autoneg_adv_reg);
	if (ret_val)
		return ret_val;

	if (phy->autoneg_mask & ADVERTISE_1000_FULL) {
		/* Read the MII 1000Base-T Control Register (Address 9). */
		ret_val = phy->ops.read_reg(hw, PHY_1000T_CTRL,
					    &mii_1000t_ctrl_reg);
		if (ret_val)
			return ret_val;
	}

	/* Need to parse both autoneg_advertised and fc and set up
	 * the appropriate PHY registers.  First we will parse for
	 * autoneg_advertised software override.  Since we can advertise
	 * a plethora of combinations, we need to check each bit
	 * individually.
	 */

	/* First we clear all the 10/100 mb speed bits in the Auto-Neg
	 * Advertisement Register (Address 4) and the 1000 mb speed bits in
	 * the  1000Base-T Control Register (Address 9).
	 */
	mii_autoneg_adv_reg &= ~(NWAY_AR_100TX_FD_CAPS |
				 NWAY_AR_100TX_HD_CAPS |
				 NWAY_AR_10T_FD_CAPS   |
				 NWAY_AR_10T_HD_CAPS);
	mii_1000t_ctrl_reg &= ~(CR_1000T_HD_CAPS | CR_1000T_FD_CAPS);

	DEBUGOUT1("autoneg_advertised %x\n", phy->autoneg_advertised);

	/* Do we want to advertise 10 Mb Half Duplex? */
	if (phy->autoneg_advertised & ADVERTISE_10_HALF) {
		DEBUGOUT("Advertise 10mb Half duplex\n");
		mii_autoneg_adv_reg |= NWAY_AR_10T_HD_CAPS;
	}

	/* Do we want to advertise 10 Mb Full Duplex? */
	if (phy->autoneg_advertised & ADVERTISE_10_FULL) {
		DEBUGOUT("Advertise 10mb Full duplex\n");
		mii_autoneg_adv_reg |= NWAY_AR_10T_FD_CAPS;
	}

	/* Do we want to advertise 100 Mb Half Duplex? */
	if (phy->autoneg_advertised & ADVERTISE_100_HALF) {
		DEBUGOUT("Advertise 100mb Half duplex\n");
		mii_autoneg_adv_reg |= NWAY_AR_100TX_HD_CAPS;
	}

	/* Do we want to advertise 100 Mb Full Duplex? */
	if (phy->autoneg_advertised & ADVERTISE_100_FULL) {
		DEBUGOUT("Advertise 100mb Full duplex\n");
		mii_autoneg_adv_reg |= NWAY_AR_100TX_FD_CAPS;
	}

	/* We do not allow the Phy to advertise 1000 Mb Half Duplex */
	if (phy->autoneg_advertised & ADVERTISE_1000_HALF)
		DEBUGOUT("Advertise 1000mb Half duplex request denied!\n");

	/* Do we want to advertise 1000 Mb Full Duplex? */
	if (phy->autoneg_advertised & ADVERTISE_1000_FULL) {
		DEBUGOUT("Advertise 1000mb Full duplex\n");
		mii_1000t_ctrl_reg |= CR_1000T_FD_CAPS;
	}

	/* Check for a software override of the flow control settings, and
	 * setup the PHY advertisement registers accordingly.  If
	 * auto-negotiation is enabled, then software will have to set the
	 * "PAUSE" bits to the correct value in the Auto-Negotiation
	 * Advertisement Register (PHY_AUTONEG_ADV) and re-start auto-
	 * negotiation.
	 *
	 * The possible values of the "fc" parameter are:
	 *      0:  Flow control is completely disabled
	 *      1:  Rx flow control is enabled (we can receive pause frames
	 *          but not send pause frames).
	 *      2:  Tx flow control is enabled (we can send pause frames
	 *          but we do not support receiving pause frames).
	 *      3:  Both Rx and Tx flow control (symmetric) are enabled.
	 *  other:  No software override.  The flow control configuration
	 *          in the EEPROM is used.
	 */
	switch (hw->fc.current_mode) {
	case e1000_fc_none:
		/* Flow control (Rx & Tx) is completely disabled by a
		 * software over-ride.
		 */
		mii_autoneg_adv_reg &= ~(NWAY_AR_ASM_DIR | NWAY_AR_PAUSE);
		break;
	case e1000_fc_rx_pause:
		/* Rx Flow control is enabled, and Tx Flow control is
		 * disabled, by a software over-ride.
		 *
		 * Since there really isn't a way to advertise that we are
		 * capable of Rx Pause ONLY, we will advertise that we
		 * support both symmetric and asymmetric Rx PAUSE.  Later
		 * (in e1000_config_fc_after_link_up) we will disable the
		 * hw's ability to send PAUSE frames.
		 */
		mii_autoneg_adv_reg |= (NWAY_AR_ASM_DIR | NWAY_AR_PAUSE);
		break;
	case e1000_fc_tx_pause:
		/* Tx Flow control is enabled, and Rx Flow control is
		 * disabled, by a software over-ride.
		 */
		mii_autoneg_adv_reg |= NWAY_AR_ASM_DIR;
		mii_autoneg_adv_reg &= ~NWAY_AR_PAUSE;
		break;
	case e1000_fc_full:
		/* Flow control (both Rx and Tx) is enabled by a software
		 * over-ride.
		 */
		mii_autoneg_adv_reg |= (NWAY_AR_ASM_DIR | NWAY_AR_PAUSE);
		break;
	default:
		DEBUGOUT("Flow control param set incorrectly\n");
		return -E1000_ERR_CONFIG;
	}

	ret_val = phy->ops.write_reg(hw, PHY_AUTONEG_ADV, mii_autoneg_adv_reg);
	if (ret_val)
		return ret_val;

	DEBUGOUT1("Auto-Neg Advertising %x\n", mii_autoneg_adv_reg);

	if (phy->autoneg_mask & ADVERTISE_1000_FULL)
		ret_val = phy->ops.write_reg(hw, PHY_1000T_CTRL,
					     mii_1000t_ctrl_reg);

	return ret_val;
}

/**
 *  e1000_copper_link_autoneg - Setup/Enable autoneg for copper link
 *  @hw: pointer to the HW structure
 *
 *  Performs initial bounds checking on autoneg advertisement parameter, then
 *  configure to advertise the full capability.  Setup the PHY to autoneg
 *  and restart the negotiation process between the link partner.  If
 *  autoneg_wait_to_complete, then wait for autoneg to complete before exiting.
 **/
static s32 e1000_copper_link_autoneg(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_ctrl;

	DEBUGFUNC("e1000_copper_link_autoneg");

	/* Perform some bounds checking on the autoneg advertisement
	 * parameter.
	 */
	phy->autoneg_advertised &= phy->autoneg_mask;

	/* If autoneg_advertised is zero, we assume it was not defaulted
	 * by the calling code so we set to advertise full capability.
	 */
	if (!phy->autoneg_advertised)
		phy->autoneg_advertised = phy->autoneg_mask;

	DEBUGOUT("Reconfiguring auto-neg advertisement params\n");
	ret_val = e1000_phy_setup_autoneg(hw);
	if (ret_val) {
		DEBUGOUT("Error Setting up Auto-Negotiation\n");
		return ret_val;
	}
	DEBUGOUT("Restarting Auto-Neg\n");

	/* Restart auto-negotiation by setting the Auto Neg Enable bit and
	 * the Auto Neg Restart bit in the PHY control register.
	 */
	ret_val = phy->ops.read_reg(hw, PHY_CONTROL, &phy_ctrl);
	if (ret_val)
		return ret_val;

	phy_ctrl |= (MII_CR_AUTO_NEG_EN | MII_CR_RESTART_AUTO_NEG);
	ret_val = phy->ops.write_reg(hw, PHY_CONTROL, phy_ctrl);
	if (ret_val)
		return ret_val;

	/* Does the user want to wait for Auto-Neg to complete here, or
	 * check at a later time (for example, callback routine).
	 */
	if (phy->autoneg_wait_to_complete) {
		ret_val = e1000_wait_autoneg(hw);
		if (ret_val) {
			DEBUGOUT("Error while waiting for autoneg to complete\n");
			return ret_val;
		}
	}

	hw->mac.get_link_status = true;

	return ret_val;
}

/**
 *  e1000_setup_copper_link_generic - Configure copper link settings
 *  @hw: pointer to the HW structure
 *
 *  Calls the appropriate function to configure the link for auto-neg or forced
 *  speed and duplex.  Then we check for link, once link is established calls
 *  to configure collision distance and flow control are called.  If link is
 *  not established, we return -E1000_ERR_PHY (-2).
 **/
s32 e1000_setup_copper_link_generic(struct e1000_hw *hw)
{
	s32 ret_val;
	bool link;

	DEBUGFUNC("e1000_setup_copper_link_generic");

	if (hw->mac.autoneg) {
		/* Setup autoneg and flow control advertisement and perform
		 * autonegotiation.
		 */
		ret_val = e1000_copper_link_autoneg(hw);
		if (ret_val)
			return ret_val;
	} else {
		/* PHY will be set to 10H, 10F, 100H or 100F
		 * depending on user settings.
		 */
		DEBUGOUT("Forcing Speed and Duplex\n");
		ret_val = hw->phy.ops.force_speed_duplex(hw);
		if (ret_val) {
			DEBUGOUT("Error Forcing Speed and Duplex\n");
			return ret_val;
		}
	}

	/* Check link status. Wait up to 100 microseconds for link to become
	 * valid.
	 */
	ret_val = e1000_phy_has_link_generic(hw, COPPER_LINK_UP_LIMIT, 10,
					     &link);
	if (ret_val)
		return ret_val;

	if (link) {
		DEBUGOUT("Valid link established!!!\n");
		hw->mac.ops.config_collision_dist(hw);
		ret_val = e1000_config_fc_after_link_up_generic(hw);
	} else {
		DEBUGOUT("Unable to establish link!!!\n");
	}

	return ret_val;
}

/**
 *  e1000_phy_force_speed_duplex_igp - Force speed/duplex for igp PHY
 *  @hw: pointer to the HW structure
 *
 *  Calls the PHY setup function to force speed and duplex.  Clears the
 *  auto-crossover to force MDI manually.  Waits for link and returns
 *  successful if link up is successful, else -E1000_ERR_PHY (-2).
 **/
s32 e1000_phy_force_speed_duplex_igp(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data;
	bool link;

	DEBUGFUNC("e1000_phy_force_speed_duplex_igp");

	ret_val = phy->ops.read_reg(hw, PHY_CONTROL, &phy_data);
	if (ret_val)
		return ret_val;

	e1000_phy_force_speed_duplex_setup(hw, &phy_data);

	ret_val = phy->ops.write_reg(hw, PHY_CONTROL, phy_data);
	if (ret_val)
		return ret_val;

	/* Clear Auto-Crossover to force MDI manually.  IGP requires MDI
	 * forced whenever speed and duplex are forced.
	 */
	ret_val = phy->ops.read_reg(hw, IGP01E1000_PHY_PORT_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	phy_data &= ~IGP01E1000_PSCR_AUTO_MDIX;
	phy_data &= ~IGP01E1000_PSCR_FORCE_MDI_MDIX;

	ret_val = phy->ops.write_reg(hw, IGP01E1000_PHY_PORT_CTRL, phy_data);
	if (ret_val)
		return ret_val;

	DEBUGOUT1("IGP PSCR: %X\n", phy_data);

	usec_delay(1);

	if (phy->autoneg_wait_to_complete) {
		DEBUGOUT("Waiting for forced speed/duplex link on IGP phy.\n");

		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
		if (ret_val)
			return ret_val;

		if (!link)
			DEBUGOUT("Link taking longer than expected.\n");

		/* Try once more */
		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
	}

	return ret_val;
}

/**
 *  e1000_phy_force_speed_duplex_m88 - Force speed/duplex for m88 PHY
 *  @hw: pointer to the HW structure
 *
 *  Calls the PHY setup function to force speed and duplex.  Clears the
 *  auto-crossover to force MDI manually.  Resets the PHY to commit the
 *  changes.  If time expires while waiting for link up, we reset the DSP.
 *  After reset, TX_CLK and CRS on Tx must be set.  Return successful upon
 *  successful completion, else return corresponding error code.
 **/
s32 e1000_phy_force_speed_duplex_m88(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data;
	bool link;

	DEBUGFUNC("e1000_phy_force_speed_duplex_m88");

	/* I210 and I211 devices support Auto-Crossover in forced operation. */
	if (phy->type != e1000_phy_i210) {
		/* Clear Auto-Crossover to force MDI manually.  M88E1000
		 * requires MDI forced whenever speed and duplex are forced.
		 */
		ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_CTRL,
					    &phy_data);
		if (ret_val)
			return ret_val;

		phy_data &= ~M88E1000_PSCR_AUTO_X_MODE;
		ret_val = phy->ops.write_reg(hw, M88E1000_PHY_SPEC_CTRL,
					     phy_data);
		if (ret_val)
			return ret_val;

		DEBUGOUT1("M88E1000 PSCR: %X\n", phy_data);
	}

	ret_val = phy->ops.read_reg(hw, PHY_CONTROL, &phy_data);
	if (ret_val)
		return ret_val;

	e1000_phy_force_speed_duplex_setup(hw, &phy_data);

	ret_val = phy->ops.write_reg(hw, PHY_CONTROL, phy_data);
	if (ret_val)
		return ret_val;

	/* Reset the phy to commit changes. */
	ret_val = hw->phy.ops.commit(hw);
	if (ret_val)
		return ret_val;

	if (phy->autoneg_wait_to_complete) {
		DEBUGOUT("Waiting for forced speed/duplex link on M88 phy.\n");

		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
		if (ret_val)
			return ret_val;

		if (!link) {
			bool reset_dsp = true;

			switch (hw->phy.id) {
			case I347AT4_E_PHY_ID:
			case M88E1340M_E_PHY_ID:
			case M88E1112_E_PHY_ID:
			case M88E1543_E_PHY_ID:
			case M88E1512_E_PHY_ID:
			case I210_I_PHY_ID:
				reset_dsp = false;
				break;
			default:
				if (hw->phy.type != e1000_phy_m88)
					reset_dsp = false;
				break;
			}

			if (!reset_dsp) {
				DEBUGOUT("Link taking longer than expected.\n");
			} else {
				/* We didn't get link.
				 * Reset the DSP and cross our fingers.
				 */
				ret_val = phy->ops.write_reg(hw,
						M88E1000_PHY_PAGE_SELECT,
						0x001d);
				if (ret_val)
					return ret_val;
				ret_val = e1000_phy_reset_dsp_generic(hw);
				if (ret_val)
					return ret_val;
			}
		}

		/* Try once more */
		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
		if (ret_val)
			return ret_val;
	}

	if (hw->phy.type != e1000_phy_m88)
		return E1000_SUCCESS;

	if (hw->phy.id == I347AT4_E_PHY_ID ||
		hw->phy.id == M88E1340M_E_PHY_ID ||
		hw->phy.id == M88E1112_E_PHY_ID)
		return E1000_SUCCESS;
	if (hw->phy.id == I210_I_PHY_ID)
		return E1000_SUCCESS;
	if ((hw->phy.id == M88E1543_E_PHY_ID) ||
	    (hw->phy.id == M88E1512_E_PHY_ID))
		return E1000_SUCCESS;
	ret_val = phy->ops.read_reg(hw, M88E1000_EXT_PHY_SPEC_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	/* Resetting the phy means we need to re-force TX_CLK in the
	 * Extended PHY Specific Control Register to 25MHz clock from
	 * the reset value of 2.5MHz.
	 */
	phy_data |= M88E1000_EPSCR_TX_CLK_25;
	ret_val = phy->ops.write_reg(hw, M88E1000_EXT_PHY_SPEC_CTRL, phy_data);
	if (ret_val)
		return ret_val;

	/* In addition, we must re-enable CRS on Tx for both half and full
	 * duplex.
	 */
	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	phy_data |= M88E1000_PSCR_ASSERT_CRS_ON_TX;
	ret_val = phy->ops.write_reg(hw, M88E1000_PHY_SPEC_CTRL, phy_data);

	return ret_val;
}

/**
 *  e1000_phy_force_speed_duplex_ife - Force PHY speed & duplex
 *  @hw: pointer to the HW structure
 *
 *  Forces the speed and duplex settings of the PHY.
 *  This is a function pointer entry point only called by
 *  PHY setup routines.
 **/
s32 e1000_phy_force_speed_duplex_ife(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;
	bool link;

	DEBUGFUNC("e1000_phy_force_speed_duplex_ife");

	ret_val = phy->ops.read_reg(hw, PHY_CONTROL, &data);
	if (ret_val)
		return ret_val;

	e1000_phy_force_speed_duplex_setup(hw, &data);

	ret_val = phy->ops.write_reg(hw, PHY_CONTROL, data);
	if (ret_val)
		return ret_val;

	/* Disable MDI-X support for 10/100 */
	ret_val = phy->ops.read_reg(hw, IFE_PHY_MDIX_CONTROL, &data);
	if (ret_val)
		return ret_val;

	data &= ~IFE_PMC_AUTO_MDIX;
	data &= ~IFE_PMC_FORCE_MDIX;

	ret_val = phy->ops.write_reg(hw, IFE_PHY_MDIX_CONTROL, data);
	if (ret_val)
		return ret_val;

	DEBUGOUT1("IFE PMC: %X\n", data);

	usec_delay(1);

	if (phy->autoneg_wait_to_complete) {
		DEBUGOUT("Waiting for forced speed/duplex link on IFE phy.\n");

		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
		if (ret_val)
			return ret_val;

		if (!link)
			DEBUGOUT("Link taking longer than expected.\n");

		/* Try once more */
		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
		if (ret_val)
			return ret_val;
	}

	return E1000_SUCCESS;
}

/**
 *  e1000_phy_force_speed_duplex_setup - Configure forced PHY speed/duplex
 *  @hw: pointer to the HW structure
 *  @phy_ctrl: pointer to current value of PHY_CONTROL
 *
 *  Forces speed and duplex on the PHY by doing the following: disable flow
 *  control, force speed/duplex on the MAC, disable auto speed detection,
 *  disable auto-negotiation, configure duplex, configure speed, configure
 *  the collision distance, write configuration to CTRL register.  The
 *  caller must write to the PHY_CONTROL register for these settings to
 *  take affect.
 **/
void e1000_phy_force_speed_duplex_setup(struct e1000_hw *hw, u16 *phy_ctrl)
{
	struct e1000_mac_info *mac = &hw->mac;
	u32 ctrl;

	DEBUGFUNC("e1000_phy_force_speed_duplex_setup");

	/* Turn off flow control when forcing speed/duplex */
	hw->fc.current_mode = e1000_fc_none;

	/* Force speed/duplex on the mac */
	ctrl = E1000_READ_REG(hw, E1000_CTRL);
	ctrl |= (E1000_CTRL_FRCSPD | E1000_CTRL_FRCDPX);
	ctrl &= ~E1000_CTRL_SPD_SEL;

	/* Disable Auto Speed Detection */
	ctrl &= ~E1000_CTRL_ASDE;

	/* Disable autoneg on the phy */
	*phy_ctrl &= ~MII_CR_AUTO_NEG_EN;

	/* Forcing Full or Half Duplex? */
	if (mac->forced_speed_duplex & E1000_ALL_HALF_DUPLEX) {
		ctrl &= ~E1000_CTRL_FD;
		*phy_ctrl &= ~MII_CR_FULL_DUPLEX;
		DEBUGOUT("Half Duplex\n");
	} else {
		ctrl |= E1000_CTRL_FD;
		*phy_ctrl |= MII_CR_FULL_DUPLEX;
		DEBUGOUT("Full Duplex\n");
	}

	/* Forcing 10mb or 100mb? */
	if (mac->forced_speed_duplex & E1000_ALL_100_SPEED) {
		ctrl |= E1000_CTRL_SPD_100;
		*phy_ctrl |= MII_CR_SPEED_100;
		*phy_ctrl &= ~MII_CR_SPEED_1000;
		DEBUGOUT("Forcing 100mb\n");
	} else {
		ctrl &= ~(E1000_CTRL_SPD_1000 | E1000_CTRL_SPD_100);
		*phy_ctrl &= ~(MII_CR_SPEED_1000 | MII_CR_SPEED_100);
		DEBUGOUT("Forcing 10mb\n");
	}

	hw->mac.ops.config_collision_dist(hw);

	E1000_WRITE_REG(hw, E1000_CTRL, ctrl);
}

/**
 *  e1000_set_d3_lplu_state_generic - Sets low power link up state for D3
 *  @hw: pointer to the HW structure
 *  @active: boolean used to enable/disable lplu
 *
 *  Success returns 0, Failure returns 1
 *
 *  The low power link up (lplu) state is set to the power management level D3
 *  and SmartSpeed is disabled when active is true, else clear lplu for D3
 *  and enable Smartspeed.  LPLU and Smartspeed are mutually exclusive.  LPLU
 *  is used during Dx states where the power conservation is most important.
 *  During driver activity, SmartSpeed should be enabled so performance is
 *  maintained.
 **/
s32 e1000_set_d3_lplu_state_generic(struct e1000_hw *hw, bool active)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;

	DEBUGFUNC("e1000_set_d3_lplu_state_generic");

	if (!hw->phy.ops.read_reg)
		return E1000_SUCCESS;

	ret_val = phy->ops.read_reg(hw, IGP02E1000_PHY_POWER_MGMT, &data);
	if (ret_val)
		return ret_val;

	if (!active) {
		data &= ~IGP02E1000_PM_D3_LPLU;
		ret_val = phy->ops.write_reg(hw, IGP02E1000_PHY_POWER_MGMT,
					     data);
		if (ret_val)
			return ret_val;
		/* LPLU and SmartSpeed are mutually exclusive.  LPLU is used
		 * during Dx states where the power conservation is most
		 * important.  During driver activity we should enable
		 * SmartSpeed, so performance is maintained.
		 */
		if (phy->smart_speed == e1000_smart_speed_on) {
			ret_val = phy->ops.read_reg(hw,
						    IGP01E1000_PHY_PORT_CONFIG,
						    &data);
			if (ret_val)
				return ret_val;

			data |= IGP01E1000_PSCFR_SMART_SPEED;
			ret_val = phy->ops.write_reg(hw,
						     IGP01E1000_PHY_PORT_CONFIG,
						     data);
			if (ret_val)
				return ret_val;
		} else if (phy->smart_speed == e1000_smart_speed_off) {
			ret_val = phy->ops.read_reg(hw,
						    IGP01E1000_PHY_PORT_CONFIG,
						    &data);
			if (ret_val)
				return ret_val;

			data &= ~IGP01E1000_PSCFR_SMART_SPEED;
			ret_val = phy->ops.write_reg(hw,
						     IGP01E1000_PHY_PORT_CONFIG,
						     data);
			if (ret_val)
				return ret_val;
		}
	} else if ((phy->autoneg_advertised == E1000_ALL_SPEED_DUPLEX) ||
		   (phy->autoneg_advertised == E1000_ALL_NOT_GIG) ||
		   (phy->autoneg_advertised == E1000_ALL_10_SPEED)) {
		data |= IGP02E1000_PM_D3_LPLU;
		ret_val = phy->ops.write_reg(hw, IGP02E1000_PHY_POWER_MGMT,
					     data);
		if (ret_val)
			return ret_val;

		/* When LPLU is enabled, we should disable SmartSpeed */
		ret_val = phy->ops.read_reg(hw, IGP01E1000_PHY_PORT_CONFIG,
					    &data);
		if (ret_val)
			return ret_val;

		data &= ~IGP01E1000_PSCFR_SMART_SPEED;
		ret_val = phy->ops.write_reg(hw, IGP01E1000_PHY_PORT_CONFIG,
					     data);
	}

	return ret_val;
}

/**
 *  e1000_check_downshift_generic - Checks whether a downshift in speed occurred
 *  @hw: pointer to the HW structure
 *
 *  Success returns 0, Failure returns 1
 *
 *  A downshift is detected by querying the PHY link health.
 **/
s32 e1000_check_downshift_generic(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data, offset, mask;

	DEBUGFUNC("e1000_check_downshift_generic");

	switch (phy->type) {
	case e1000_phy_i210:
	case e1000_phy_m88:
	case e1000_phy_gg82563:
		offset = M88E1000_PHY_SPEC_STATUS;
		mask = M88E1000_PSSR_DOWNSHIFT;
		break;
	case e1000_phy_igp_2:
	case e1000_phy_igp_3:
		offset = IGP01E1000_PHY_LINK_HEALTH;
		mask = IGP01E1000_PLHR_SS_DOWNGRADE;
		break;
	default:
		/* speed downshift not supported */
		phy->speed_downgraded = false;
		return E1000_SUCCESS;
	}

	ret_val = phy->ops.read_reg(hw, offset, &phy_data);

	if (!ret_val)
		phy->speed_downgraded = !!(phy_data & mask);

	return ret_val;
}

/**
 *  e1000_check_polarity_m88 - Checks the polarity.
 *  @hw: pointer to the HW structure
 *
 *  Success returns 0, Failure returns -E1000_ERR_PHY (-2)
 *
 *  Polarity is determined based on the PHY specific status register.
 **/
s32 e1000_check_polarity_m88(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;

	DEBUGFUNC("e1000_check_polarity_m88");

	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_STATUS, &data);

	if (!ret_val)
		phy->cable_polarity = ((data & M88E1000_PSSR_REV_POLARITY)
				       ? e1000_rev_polarity_reversed
				       : e1000_rev_polarity_normal);

	return ret_val;
}

/**
 *  e1000_check_polarity_igp - Checks the polarity.
 *  @hw: pointer to the HW structure
 *
 *  Success returns 0, Failure returns -E1000_ERR_PHY (-2)
 *
 *  Polarity is determined based on the PHY port status register, and the
 *  current speed (since there is no polarity at 100Mbps).
 **/
s32 e1000_check_polarity_igp(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data, offset, mask;

	DEBUGFUNC("e1000_check_polarity_igp");

	/* Polarity is determined based on the speed of
	 * our connection.
	 */
	ret_val = phy->ops.read_reg(hw, IGP01E1000_PHY_PORT_STATUS, &data);
	if (ret_val)
		return ret_val;

	if ((data & IGP01E1000_PSSR_SPEED_MASK) ==
	    IGP01E1000_PSSR_SPEED_1000MBPS) {
		offset = IGP01E1000_PHY_PCS_INIT_REG;
		mask = IGP01E1000_PHY_POLARITY_MASK;
	} else {
		/* This really only applies to 10Mbps since
		 * there is no polarity for 100Mbps (always 0).
		 */
		offset = IGP01E1000_PHY_PORT_STATUS;
		mask = IGP01E1000_PSSR_POLARITY_REVERSED;
	}

	ret_val = phy->ops.read_reg(hw, offset, &data);

	if (!ret_val)
		phy->cable_polarity = ((data & mask)
				       ? e1000_rev_polarity_reversed
				       : e1000_rev_polarity_normal);

	return ret_val;
}

/**
 *  e1000_check_polarity_ife - Check cable polarity for IFE PHY
 *  @hw: pointer to the HW structure
 *
 *  Polarity is determined on the polarity reversal feature being enabled.
 **/
s32 e1000_check_polarity_ife(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data, offset, mask;

	DEBUGFUNC("e1000_check_polarity_ife");

	/* Polarity is determined based on the reversal feature being enabled.
	 */
	if (phy->polarity_correction) {
		offset = IFE_PHY_EXTENDED_STATUS_CONTROL;
		mask = IFE_PESC_POLARITY_REVERSED;
	} else {
		offset = IFE_PHY_SPECIAL_CONTROL;
		mask = IFE_PSC_FORCE_POLARITY;
	}

	ret_val = phy->ops.read_reg(hw, offset, &phy_data);

	if (!ret_val)
		phy->cable_polarity = ((phy_data & mask)
				       ? e1000_rev_polarity_reversed
				       : e1000_rev_polarity_normal);

	return ret_val;
}

/**
 *  e1000_wait_autoneg - Wait for auto-neg completion
 *  @hw: pointer to the HW structure
 *
 *  Waits for auto-negotiation to complete or for the auto-negotiation time
 *  limit to expire, which ever happens first.
 **/
static s32 e1000_wait_autoneg(struct e1000_hw *hw)
{
	s32 ret_val = E1000_SUCCESS;
	u16 i, phy_status;

	DEBUGFUNC("e1000_wait_autoneg");

	if (!hw->phy.ops.read_reg)
		return E1000_SUCCESS;

	/* Break after autoneg completes or PHY_AUTO_NEG_LIMIT expires. */
	for (i = PHY_AUTO_NEG_LIMIT; i > 0; i--) {
		ret_val = hw->phy.ops.read_reg(hw, PHY_STATUS, &phy_status);
		if (ret_val)
			break;
		ret_val = hw->phy.ops.read_reg(hw, PHY_STATUS, &phy_status);
		if (ret_val)
			break;
		if (phy_status & MII_SR_AUTONEG_COMPLETE)
			break;
		msec_delay(100);
	}

	/* PHY_AUTO_NEG_TIME expiration doesn't guarantee auto-negotiation
	 * has completed.
	 */
	return ret_val;
}

/**
 *  e1000_phy_has_link_generic - Polls PHY for link
 *  @hw: pointer to the HW structure
 *  @iterations: number of times to poll for link
 *  @usec_interval: delay between polling attempts
 *  @success: pointer to whether polling was successful or not
 *
 *  Polls the PHY status register for link, 'iterations' number of times.
 **/
s32 e1000_phy_has_link_generic(struct e1000_hw *hw, u32 iterations,
			       u32 usec_interval, bool *success)
{
	s32 ret_val = E1000_SUCCESS;
	u16 i, phy_status;

	DEBUGFUNC("e1000_phy_has_link_generic");

	if (!hw->phy.ops.read_reg)
		return E1000_SUCCESS;

	for (i = 0; i < iterations; i++) {
		/* Some PHYs require the PHY_STATUS register to be read
		 * twice due to the link bit being sticky.  No harm doing
		 * it across the board.
		 */
		ret_val = hw->phy.ops.read_reg(hw, PHY_STATUS, &phy_status);
		if (ret_val) {
			/* If the first read fails, another entity may have
			 * ownership of the resources, wait and try again to
			 * see if they have relinquished the resources yet.
			 */
			if (usec_interval >= 1000)
				msec_delay(usec_interval/1000);
			else
				usec_delay(usec_interval);
		}
		ret_val = hw->phy.ops.read_reg(hw, PHY_STATUS, &phy_status);
		if (ret_val)
			break;
		if (phy_status & MII_SR_LINK_STATUS)
			break;
		if (usec_interval >= 1000)
			msec_delay(usec_interval/1000);
		else
			usec_delay(usec_interval);
	}

	*success = (i < iterations);

	return ret_val;
}

/**
 *  e1000_get_cable_length_m88 - Determine cable length for m88 PHY
 *  @hw: pointer to the HW structure
 *
 *  Reads the PHY specific status register to retrieve the cable length
 *  information.  The cable length is determined by averaging the minimum and
 *  maximum values to get the "average" cable length.  The m88 PHY has four
 *  possible cable length values, which are:
 *	Register Value		Cable Length
 *	0			< 50 meters
 *	1			50 - 80 meters
 *	2			80 - 110 meters
 *	3			110 - 140 meters
 *	4			> 140 meters
 **/
s32 e1000_get_cable_length_m88(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data, index;

	DEBUGFUNC("e1000_get_cable_length_m88");

	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_STATUS, &phy_data);
	if (ret_val)
		return ret_val;

	index = ((phy_data & M88E1000_PSSR_CABLE_LENGTH) >>
		 M88E1000_PSSR_CABLE_LENGTH_SHIFT);

	if (index >= M88E1000_CABLE_LENGTH_TABLE_SIZE - 1)
		return -E1000_ERR_PHY;

	phy->min_cable_length = e1000_m88_cable_length_table[index];
	phy->max_cable_length = e1000_m88_cable_length_table[index + 1];

	phy->cable_length = (phy->min_cable_length + phy->max_cable_length) / 2;

	return E1000_SUCCESS;
}

s32 e1000_get_cable_length_m88_gen2(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data, phy_data2, is_cm;
	u16 index, default_page;

	DEBUGFUNC("e1000_get_cable_length_m88_gen2");

	switch (hw->phy.id) {
	case I210_I_PHY_ID:
		/* Get cable length from PHY Cable Diagnostics Control Reg */
		ret_val = phy->ops.read_reg(hw, (0x7 << GS40G_PAGE_SHIFT) +
					    (I347AT4_PCDL + phy->addr),
					    &phy_data);
		if (ret_val)
			return ret_val;

		/* Check if the unit of cable length is meters or cm */
		ret_val = phy->ops.read_reg(hw, (0x7 << GS40G_PAGE_SHIFT) +
					    I347AT4_PCDC, &phy_data2);
		if (ret_val)
			return ret_val;

		is_cm = !(phy_data2 & I347AT4_PCDC_CABLE_LENGTH_UNIT);

		/* Populate the phy structure with cable length in meters */
		phy->min_cable_length = phy_data / (is_cm ? 100 : 1);
		phy->max_cable_length = phy_data / (is_cm ? 100 : 1);
		phy->cable_length = phy_data / (is_cm ? 100 : 1);
		break;
	case M88E1543_E_PHY_ID:
	case M88E1512_E_PHY_ID:
	case M88E1340M_E_PHY_ID:
	case I347AT4_E_PHY_ID:
		/* Remember the original page select and set it to 7 */
		ret_val = phy->ops.read_reg(hw, I347AT4_PAGE_SELECT,
					    &default_page);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.write_reg(hw, I347AT4_PAGE_SELECT, 0x07);
		if (ret_val)
			return ret_val;

		/* Get cable length from PHY Cable Diagnostics Control Reg */
		ret_val = phy->ops.read_reg(hw, (I347AT4_PCDL + phy->addr),
					    &phy_data);
		if (ret_val)
			return ret_val;

		/* Check if the unit of cable length is meters or cm */
		ret_val = phy->ops.read_reg(hw, I347AT4_PCDC, &phy_data2);
		if (ret_val)
			return ret_val;

		is_cm = !(phy_data2 & I347AT4_PCDC_CABLE_LENGTH_UNIT);

		/* Populate the phy structure with cable length in meters */
		phy->min_cable_length = phy_data / (is_cm ? 100 : 1);
		phy->max_cable_length = phy_data / (is_cm ? 100 : 1);
		phy->cable_length = phy_data / (is_cm ? 100 : 1);

		/* Reset the page select to its original value */
		ret_val = phy->ops.write_reg(hw, I347AT4_PAGE_SELECT,
					     default_page);
		if (ret_val)
			return ret_val;
		break;

	case M88E1112_E_PHY_ID:
		/* Remember the original page select and set it to 5 */
		ret_val = phy->ops.read_reg(hw, I347AT4_PAGE_SELECT,
					    &default_page);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.write_reg(hw, I347AT4_PAGE_SELECT, 0x05);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.read_reg(hw, M88E1112_VCT_DSP_DISTANCE,
					    &phy_data);
		if (ret_val)
			return ret_val;

		index = (phy_data & M88E1000_PSSR_CABLE_LENGTH) >>
			M88E1000_PSSR_CABLE_LENGTH_SHIFT;

		if (index >= M88E1000_CABLE_LENGTH_TABLE_SIZE - 1)
			return -E1000_ERR_PHY;

		phy->min_cable_length = e1000_m88_cable_length_table[index];
		phy->max_cable_length = e1000_m88_cable_length_table[index + 1];

		phy->cable_length = (phy->min_cable_length +
				     phy->max_cable_length) / 2;

		/* Reset the page select to its original value */
		ret_val = phy->ops.write_reg(hw, I347AT4_PAGE_SELECT,
					     default_page);
		if (ret_val)
			return ret_val;

		break;
	default:
		return -E1000_ERR_PHY;
	}

	return ret_val;
}

/**
 *  e1000_get_cable_length_igp_2 - Determine cable length for igp2 PHY
 *  @hw: pointer to the HW structure
 *
 *  The automatic gain control (agc) normalizes the amplitude of the
 *  received signal, adjusting for the attenuation produced by the
 *  cable.  By reading the AGC registers, which represent the
 *  combination of coarse and fine gain value, the value can be put
 *  into a lookup table to obtain the approximate cable length
 *  for each channel.
 **/
s32 e1000_get_cable_length_igp_2(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data, i, agc_value = 0;
	u16 cur_agc_index, max_agc_index = 0;
	u16 min_agc_index = IGP02E1000_CABLE_LENGTH_TABLE_SIZE - 1;
	static const u16 agc_reg_array[IGP02E1000_PHY_CHANNEL_NUM] = {
		IGP02E1000_PHY_AGC_A,
		IGP02E1000_PHY_AGC_B,
		IGP02E1000_PHY_AGC_C,
		IGP02E1000_PHY_AGC_D
	};

	DEBUGFUNC("e1000_get_cable_length_igp_2");

	/* Read the AGC registers for all channels */
	for (i = 0; i < IGP02E1000_PHY_CHANNEL_NUM; i++) {
		ret_val = phy->ops.read_reg(hw, agc_reg_array[i], &phy_data);
		if (ret_val)
			return ret_val;

		/* Getting bits 15:9, which represent the combination of
		 * coarse and fine gain values.  The result is a number
		 * that can be put into the lookup table to obtain the
		 * approximate cable length.
		 */
		cur_agc_index = ((phy_data >> IGP02E1000_AGC_LENGTH_SHIFT) &
				 IGP02E1000_AGC_LENGTH_MASK);

		/* Array index bound check. */
		if ((cur_agc_index >= IGP02E1000_CABLE_LENGTH_TABLE_SIZE) ||
		    (cur_agc_index == 0))
			return -E1000_ERR_PHY;

		/* Remove min & max AGC values from calculation. */
		if (e1000_igp_2_cable_length_table[min_agc_index] >
		    e1000_igp_2_cable_length_table[cur_agc_index])
			min_agc_index = cur_agc_index;
		if (e1000_igp_2_cable_length_table[max_agc_index] <
		    e1000_igp_2_cable_length_table[cur_agc_index])
			max_agc_index = cur_agc_index;

		agc_value += e1000_igp_2_cable_length_table[cur_agc_index];
	}

	agc_value -= (e1000_igp_2_cable_length_table[min_agc_index] +
		      e1000_igp_2_cable_length_table[max_agc_index]);
	agc_value /= (IGP02E1000_PHY_CHANNEL_NUM - 2);

	/* Calculate cable length with the error range of +/- 10 meters. */
	phy->min_cable_length = (((agc_value - IGP02E1000_AGC_RANGE) > 0) ?
				 (agc_value - IGP02E1000_AGC_RANGE) : 0);
	phy->max_cable_length = agc_value + IGP02E1000_AGC_RANGE;

	phy->cable_length = (phy->min_cable_length + phy->max_cable_length) / 2;

	return E1000_SUCCESS;
}

/**
 *  e1000_get_phy_info_m88 - Retrieve PHY information
 *  @hw: pointer to the HW structure
 *
 *  Valid for only copper links.  Read the PHY status register (sticky read)
 *  to verify that link is up.  Read the PHY special control register to
 *  determine the polarity and 10base-T extended distance.  Read the PHY
 *  special status register to determine MDI/MDIx and current speed.  If
 *  speed is 1000, then determine cable length, local and remote receiver.
 **/
s32 e1000_get_phy_info_m88(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32  ret_val;
	u16 phy_data;
	bool link;

	DEBUGFUNC("e1000_get_phy_info_m88");

	if (phy->media_type != e1000_media_type_copper) {
		DEBUGOUT("Phy info is only valid for copper media\n");
		return -E1000_ERR_CONFIG;
	}

	ret_val = e1000_phy_has_link_generic(hw, 1, 0, &link);
	if (ret_val)
		return ret_val;

	if (!link) {
		DEBUGOUT("Phy info is only valid if link is up\n");
		return -E1000_ERR_CONFIG;
	}

	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_CTRL, &phy_data);
	if (ret_val)
		return ret_val;

	phy->polarity_correction = !!(phy_data &
				      M88E1000_PSCR_POLARITY_REVERSAL);

	ret_val = e1000_check_polarity_m88(hw);
	if (ret_val)
		return ret_val;

	ret_val = phy->ops.read_reg(hw, M88E1000_PHY_SPEC_STATUS, &phy_data);
	if (ret_val)
		return ret_val;

	phy->is_mdix = !!(phy_data & M88E1000_PSSR_MDIX);

	if ((phy_data & M88E1000_PSSR_SPEED) == M88E1000_PSSR_1000MBS) {
		ret_val = hw->phy.ops.get_cable_length(hw);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.read_reg(hw, PHY_1000T_STATUS, &phy_data);
		if (ret_val)
			return ret_val;

		phy->local_rx = (phy_data & SR_1000T_LOCAL_RX_STATUS)
				? e1000_1000t_rx_status_ok
				: e1000_1000t_rx_status_not_ok;

		phy->remote_rx = (phy_data & SR_1000T_REMOTE_RX_STATUS)
				 ? e1000_1000t_rx_status_ok
				 : e1000_1000t_rx_status_not_ok;
	} else {
		/* Set values to "undefined" */
		phy->cable_length = E1000_CABLE_LENGTH_UNDEFINED;
		phy->local_rx = e1000_1000t_rx_status_undefined;
		phy->remote_rx = e1000_1000t_rx_status_undefined;
	}

	return ret_val;
}

/**
 *  e1000_get_phy_info_igp - Retrieve igp PHY information
 *  @hw: pointer to the HW structure
 *
 *  Read PHY status to determine if link is up.  If link is up, then
 *  set/determine 10base-T extended distance and polarity correction.  Read
 *  PHY port status to determine MDI/MDIx and speed.  Based on the speed,
 *  determine on the cable length, local and remote receiver.
 **/
s32 e1000_get_phy_info_igp(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;
	bool link;

	DEBUGFUNC("e1000_get_phy_info_igp");

	ret_val = e1000_phy_has_link_generic(hw, 1, 0, &link);
	if (ret_val)
		return ret_val;

	if (!link) {
		DEBUGOUT("Phy info is only valid if link is up\n");
		return -E1000_ERR_CONFIG;
	}

	phy->polarity_correction = true;

	ret_val = e1000_check_polarity_igp(hw);
	if (ret_val)
		return ret_val;

	ret_val = phy->ops.read_reg(hw, IGP01E1000_PHY_PORT_STATUS, &data);
	if (ret_val)
		return ret_val;

	phy->is_mdix = !!(data & IGP01E1000_PSSR_MDIX);

	if ((data & IGP01E1000_PSSR_SPEED_MASK) ==
	    IGP01E1000_PSSR_SPEED_1000MBPS) {
		ret_val = phy->ops.get_cable_length(hw);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.read_reg(hw, PHY_1000T_STATUS, &data);
		if (ret_val)
			return ret_val;

		phy->local_rx = (data & SR_1000T_LOCAL_RX_STATUS)
				? e1000_1000t_rx_status_ok
				: e1000_1000t_rx_status_not_ok;

		phy->remote_rx = (data & SR_1000T_REMOTE_RX_STATUS)
				 ? e1000_1000t_rx_status_ok
				 : e1000_1000t_rx_status_not_ok;
	} else {
		phy->cable_length = E1000_CABLE_LENGTH_UNDEFINED;
		phy->local_rx = e1000_1000t_rx_status_undefined;
		phy->remote_rx = e1000_1000t_rx_status_undefined;
	}

	return ret_val;
}

/**
 *  e1000_get_phy_info_ife - Retrieves various IFE PHY states
 *  @hw: pointer to the HW structure
 *
 *  Populates "phy" structure with various feature states.
 **/
s32 e1000_get_phy_info_ife(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;
	bool link;

	DEBUGFUNC("e1000_get_phy_info_ife");

	ret_val = e1000_phy_has_link_generic(hw, 1, 0, &link);
	if (ret_val)
		return ret_val;

	if (!link) {
		DEBUGOUT("Phy info is only valid if link is up\n");
		return -E1000_ERR_CONFIG;
	}

	ret_val = phy->ops.read_reg(hw, IFE_PHY_SPECIAL_CONTROL, &data);
	if (ret_val)
		return ret_val;
	phy->polarity_correction = !(data & IFE_PSC_AUTO_POLARITY_DISABLE);

	if (phy->polarity_correction) {
		ret_val = e1000_check_polarity_ife(hw);
		if (ret_val)
			return ret_val;
	} else {
		/* Polarity is forced */
		phy->cable_polarity = ((data & IFE_PSC_FORCE_POLARITY)
				       ? e1000_rev_polarity_reversed
				       : e1000_rev_polarity_normal);
	}

	ret_val = phy->ops.read_reg(hw, IFE_PHY_MDIX_CONTROL, &data);
	if (ret_val)
		return ret_val;

	phy->is_mdix = !!(data & IFE_PMC_MDIX_STATUS);

	/* The following parameters are undefined for 10/100 operation. */
	phy->cable_length = E1000_CABLE_LENGTH_UNDEFINED;
	phy->local_rx = e1000_1000t_rx_status_undefined;
	phy->remote_rx = e1000_1000t_rx_status_undefined;

	return E1000_SUCCESS;
}

/**
 *  e1000_phy_sw_reset_generic - PHY software reset
 *  @hw: pointer to the HW structure
 *
 *  Does a software reset of the PHY by reading the PHY control register and
 *  setting/write the control register reset bit to the PHY.
 **/
s32 e1000_phy_sw_reset_generic(struct e1000_hw *hw)
{
	s32 ret_val;
	u16 phy_ctrl;

	DEBUGFUNC("e1000_phy_sw_reset_generic");

	if (!hw->phy.ops.read_reg)
		return E1000_SUCCESS;

	ret_val = hw->phy.ops.read_reg(hw, PHY_CONTROL, &phy_ctrl);
	if (ret_val)
		return ret_val;

	phy_ctrl |= MII_CR_RESET;
	ret_val = hw->phy.ops.write_reg(hw, PHY_CONTROL, phy_ctrl);
	if (ret_val)
		return ret_val;

	usec_delay(1);

	return ret_val;
}

/**
 *  e1000_phy_hw_reset_generic - PHY hardware reset
 *  @hw: pointer to the HW structure
 *
 *  Verify the reset block is not blocking us from resetting.  Acquire
 *  semaphore (if necessary) and read/set/write the device control reset
 *  bit in the PHY.  Wait the appropriate delay time for the device to
 *  reset and release the semaphore (if necessary).
 **/
s32 e1000_phy_hw_reset_generic(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u32 ctrl;

	DEBUGFUNC("e1000_phy_hw_reset_generic");

	if (phy->ops.check_reset_block) {
		ret_val = phy->ops.check_reset_block(hw);
		if (ret_val)
			return E1000_SUCCESS;
	}

	ret_val = phy->ops.acquire(hw);
	if (ret_val)
		return ret_val;

	ctrl = E1000_READ_REG(hw, E1000_CTRL);
	E1000_WRITE_REG(hw, E1000_CTRL, ctrl | E1000_CTRL_PHY_RST);
	E1000_WRITE_FLUSH(hw);

	usec_delay(phy->reset_delay_us);

	E1000_WRITE_REG(hw, E1000_CTRL, ctrl);
	E1000_WRITE_FLUSH(hw);

	usec_delay(150);

	phy->ops.release(hw);

	return phy->ops.get_cfg_done(hw);
}

/**
 *  e1000_get_cfg_done_generic - Generic configuration done
 *  @hw: pointer to the HW structure
 *
 *  Generic function to wait 10 milli-seconds for configuration to complete
 *  and return success.
 **/
s32 e1000_get_cfg_done_generic(struct e1000_hw E1000_UNUSEDARG *hw)
{
	DEBUGFUNC("e1000_get_cfg_done_generic");

	msec_delay_irq(10);

	return E1000_SUCCESS;
}

/**
 *  e1000_phy_init_script_igp3 - Inits the IGP3 PHY
 *  @hw: pointer to the HW structure
 *
 *  Initializes a Intel Gigabit PHY3 when an EEPROM is not present.
 **/
s32 e1000_phy_init_script_igp3(struct e1000_hw *hw)
{
	DEBUGOUT("Running IGP 3 PHY init script\n");

	/* PHY init IGP 3 */
	/* Enable rise/fall, 10-mode work in class-A */
	hw->phy.ops.write_reg(hw, 0x2F5B, 0x9018);
	/* Remove all caps from Replica path filter */
	hw->phy.ops.write_reg(hw, 0x2F52, 0x0000);
	/* Bias trimming for ADC, AFE and Driver (Default) */
	hw->phy.ops.write_reg(hw, 0x2FB1, 0x8B24);
	/* Increase Hybrid poly bias */
	hw->phy.ops.write_reg(hw, 0x2FB2, 0xF8F0);
	/* Add 4% to Tx amplitude in Gig mode */
	hw->phy.ops.write_reg(hw, 0x2010, 0x10B0);
	/* Disable trimming (TTT) */
	hw->phy.ops.write_reg(hw, 0x2011, 0x0000);
	/* Poly DC correction to 94.6% + 2% for all channels */
	hw->phy.ops.write_reg(hw, 0x20DD, 0x249A);
	/* ABS DC correction to 95.9% */
	hw->phy.ops.write_reg(hw, 0x20DE, 0x00D3);
	/* BG temp curve trim */
	hw->phy.ops.write_reg(hw, 0x28B4, 0x04CE);
	/* Increasing ADC OPAMP stage 1 currents to max */
	hw->phy.ops.write_reg(hw, 0x2F70, 0x29E4);
	/* Force 1000 ( required for enabling PHY regs configuration) */
	hw->phy.ops.write_reg(hw, 0x0000, 0x0140);
	/* Set upd_freq to 6 */
	hw->phy.ops.write_reg(hw, 0x1F30, 0x1606);
	/* Disable NPDFE */
	hw->phy.ops.write_reg(hw, 0x1F31, 0xB814);
	/* Disable adaptive fixed FFE (Default) */
	hw->phy.ops.write_reg(hw, 0x1F35, 0x002A);
	/* Enable FFE hysteresis */
	hw->phy.ops.write_reg(hw, 0x1F3E, 0x0067);
	/* Fixed FFE for short cable lengths */
	hw->phy.ops.write_reg(hw, 0x1F54, 0x0065);
	/* Fixed FFE for medium cable lengths */
	hw->phy.ops.write_reg(hw, 0x1F55, 0x002A);
	/* Fixed FFE for long cable lengths */
	hw->phy.ops.write_reg(hw, 0x1F56, 0x002A);
	/* Enable Adaptive Clip Threshold */
	hw->phy.ops.write_reg(hw, 0x1F72, 0x3FB0);
	/* AHT reset limit to 1 */
	hw->phy.ops.write_reg(hw, 0x1F76, 0xC0FF);
	/* Set AHT master delay to 127 msec */
	hw->phy.ops.write_reg(hw, 0x1F77, 0x1DEC);
	/* Set scan bits for AHT */
	hw->phy.ops.write_reg(hw, 0x1F78, 0xF9EF);
	/* Set AHT Preset bits */
	hw->phy.ops.write_reg(hw, 0x1F79, 0x0210);
	/* Change integ_factor of channel A to 3 */
	hw->phy.ops.write_reg(hw, 0x1895, 0x0003);
	/* Change prop_factor of channels BCD to 8 */
	hw->phy.ops.write_reg(hw, 0x1796, 0x0008);
	/* Change cg_icount + enable integbp for channels BCD */
	hw->phy.ops.write_reg(hw, 0x1798, 0xD008);
	/* Change cg_icount + enable integbp + change prop_factor_master
	 * to 8 for channel A
	 */
	hw->phy.ops.write_reg(hw, 0x1898, 0xD918);
	/* Disable AHT in Slave mode on channel A */
	hw->phy.ops.write_reg(hw, 0x187A, 0x0800);
	/* Enable LPLU and disable AN to 1000 in non-D0a states,
	 * Enable SPD+B2B
	 */
	hw->phy.ops.write_reg(hw, 0x0019, 0x008D);
	/* Enable restart AN on an1000_dis change */
	hw->phy.ops.write_reg(hw, 0x001B, 0x2080);
	/* Enable wh_fifo read clock in 10/100 modes */
	hw->phy.ops.write_reg(hw, 0x0014, 0x0045);
	/* Restart AN, Speed selection is 1000 */
	hw->phy.ops.write_reg(hw, 0x0000, 0x1340);

	return E1000_SUCCESS;
}

/**
 *  e1000_get_phy_type_from_id - Get PHY type from id
 *  @phy_id: phy_id read from the phy
 *
 *  Returns the phy type from the id.
 **/
enum e1000_phy_type e1000_get_phy_type_from_id(u32 phy_id)
{
	enum e1000_phy_type phy_type = e1000_phy_unknown;

	switch (phy_id) {
	case M88E1000_I_PHY_ID:
	case M88E1000_E_PHY_ID:
	case M88E1111_I_PHY_ID:
	case M88E1011_I_PHY_ID:
	case M88E1543_E_PHY_ID:
	case M88E1512_E_PHY_ID:
	case I347AT4_E_PHY_ID:
	case M88E1112_E_PHY_ID:
	case M88E1340M_E_PHY_ID:
		phy_type = e1000_phy_m88;
		break;
	case IGP01E1000_I_PHY_ID: /* IGP 1 & 2 share this */
		phy_type = e1000_phy_igp_2;
		break;
	case GG82563_E_PHY_ID:
		phy_type = e1000_phy_gg82563;
		break;
	case IGP03E1000_E_PHY_ID:
		phy_type = e1000_phy_igp_3;
		break;
	case IFE_E_PHY_ID:
	case IFE_PLUS_E_PHY_ID:
	case IFE_C_E_PHY_ID:
		phy_type = e1000_phy_ife;
		break;
	case I82580_I_PHY_ID:
		phy_type = e1000_phy_82580;
		break;
	case I210_I_PHY_ID:
		phy_type = e1000_phy_i210;
		break;
	default:
		phy_type = e1000_phy_unknown;
		break;
	}
	return phy_type;
}

/**
 *  e1000_determine_phy_address - Determines PHY address.
 *  @hw: pointer to the HW structure
 *
 *  This uses a trial and error method to loop through possible PHY
 *  addresses. It tests each by reading the PHY ID registers and
 *  checking for a match.
 **/
s32 e1000_determine_phy_address(struct e1000_hw *hw)
{
	u32 phy_addr = 0;
	u32 i;
	enum e1000_phy_type phy_type = e1000_phy_unknown;

	hw->phy.id = phy_type;

	for (phy_addr = 0; phy_addr < E1000_MAX_PHY_ADDR; phy_addr++) {
		hw->phy.addr = phy_addr;
		i = 0;

		do {
			e1000_get_phy_id(hw);
			phy_type = e1000_get_phy_type_from_id(hw->phy.id);

			/* If phy_type is valid, break - we found our
			 * PHY address
			 */
			if (phy_type != e1000_phy_unknown)
				return E1000_SUCCESS;

			msec_delay(1);
			i++;
		} while (i < 10);
	}

	return -E1000_ERR_PHY_TYPE;
}

/**
 * e1000_power_up_phy_copper - Restore copper link in case of PHY power down
 * @hw: pointer to the HW structure
 *
 * In the case of a PHY power down to save power, or to turn off link during a
 * driver unload, or wake on lan is not enabled, restore the link to previous
 * settings.
 **/
void e1000_power_up_phy_copper(struct e1000_hw *hw)
{
	u16 mii_reg = 0;

	/* The PHY will retain its settings across a power down/up cycle */
	hw->phy.ops.read_reg(hw, PHY_CONTROL, &mii_reg);
	mii_reg &= ~MII_CR_POWER_DOWN;
	hw->phy.ops.write_reg(hw, PHY_CONTROL, mii_reg);
}

/**
 * e1000_power_down_phy_copper - Restore copper link in case of PHY power down
 * @hw: pointer to the HW structure
 *
 * In the case of a PHY power down to save power, or to turn off link during a
 * driver unload, or wake on lan is not enabled, restore the link to previous
 * settings.
 **/
void e1000_power_down_phy_copper(struct e1000_hw *hw)
{
	u16 mii_reg = 0;

	/* The PHY will retain its settings across a power down/up cycle */
	hw->phy.ops.read_reg(hw, PHY_CONTROL, &mii_reg);
	mii_reg |= MII_CR_POWER_DOWN;
	hw->phy.ops.write_reg(hw, PHY_CONTROL, mii_reg);
	msec_delay(1);
}

/**
 *  e1000_check_polarity_82577 - Checks the polarity.
 *  @hw: pointer to the HW structure
 *
 *  Success returns 0, Failure returns -E1000_ERR_PHY (-2)
 *
 *  Polarity is determined based on the PHY specific status register.
 **/
s32 e1000_check_polarity_82577(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;

	DEBUGFUNC("e1000_check_polarity_82577");

	ret_val = phy->ops.read_reg(hw, I82577_PHY_STATUS_2, &data);

	if (!ret_val)
		phy->cable_polarity = ((data & I82577_PHY_STATUS2_REV_POLARITY)
				       ? e1000_rev_polarity_reversed
				       : e1000_rev_polarity_normal);

	return ret_val;
}

/**
 *  e1000_phy_force_speed_duplex_82577 - Force speed/duplex for I82577 PHY
 *  @hw: pointer to the HW structure
 *
 *  Calls the PHY setup function to force speed and duplex.
 **/
s32 e1000_phy_force_speed_duplex_82577(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data;
	bool link;

	DEBUGFUNC("e1000_phy_force_speed_duplex_82577");

	ret_val = phy->ops.read_reg(hw, PHY_CONTROL, &phy_data);
	if (ret_val)
		return ret_val;

	e1000_phy_force_speed_duplex_setup(hw, &phy_data);

	ret_val = phy->ops.write_reg(hw, PHY_CONTROL, phy_data);
	if (ret_val)
		return ret_val;

	usec_delay(1);

	if (phy->autoneg_wait_to_complete) {
		DEBUGOUT("Waiting for forced speed/duplex link on 82577 phy\n");

		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
		if (ret_val)
			return ret_val;

		if (!link)
			DEBUGOUT("Link taking longer than expected.\n");

		/* Try once more */
		ret_val = e1000_phy_has_link_generic(hw, PHY_FORCE_LIMIT,
						     100000, &link);
	}

	return ret_val;
}

/**
 *  e1000_get_phy_info_82577 - Retrieve I82577 PHY information
 *  @hw: pointer to the HW structure
 *
 *  Read PHY status to determine if link is up.  If link is up, then
 *  set/determine 10base-T extended distance and polarity correction.  Read
 *  PHY port status to determine MDI/MDIx and speed.  Based on the speed,
 *  determine on the cable length, local and remote receiver.
 **/
s32 e1000_get_phy_info_82577(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 data;
	bool link;

	DEBUGFUNC("e1000_get_phy_info_82577");

	ret_val = e1000_phy_has_link_generic(hw, 1, 0, &link);
	if (ret_val)
		return ret_val;

	if (!link) {
		DEBUGOUT("Phy info is only valid if link is up\n");
		return -E1000_ERR_CONFIG;
	}

	phy->polarity_correction = true;

	ret_val = e1000_check_polarity_82577(hw);
	if (ret_val)
		return ret_val;

	ret_val = phy->ops.read_reg(hw, I82577_PHY_STATUS_2, &data);
	if (ret_val)
		return ret_val;

	phy->is_mdix = !!(data & I82577_PHY_STATUS2_MDIX);

	if ((data & I82577_PHY_STATUS2_SPEED_MASK) ==
	    I82577_PHY_STATUS2_SPEED_1000MBPS) {
		ret_val = hw->phy.ops.get_cable_length(hw);
		if (ret_val)
			return ret_val;

		ret_val = phy->ops.read_reg(hw, PHY_1000T_STATUS, &data);
		if (ret_val)
			return ret_val;

		phy->local_rx = (data & SR_1000T_LOCAL_RX_STATUS)
				? e1000_1000t_rx_status_ok
				: e1000_1000t_rx_status_not_ok;

		phy->remote_rx = (data & SR_1000T_REMOTE_RX_STATUS)
				 ? e1000_1000t_rx_status_ok
				 : e1000_1000t_rx_status_not_ok;
	} else {
		phy->cable_length = E1000_CABLE_LENGTH_UNDEFINED;
		phy->local_rx = e1000_1000t_rx_status_undefined;
		phy->remote_rx = e1000_1000t_rx_status_undefined;
	}

	return E1000_SUCCESS;
}

/**
 *  e1000_get_cable_length_82577 - Determine cable length for 82577 PHY
 *  @hw: pointer to the HW structure
 *
 * Reads the diagnostic status register and verifies result is valid before
 * placing it in the phy_cable_length field.
 **/
s32 e1000_get_cable_length_82577(struct e1000_hw *hw)
{
	struct e1000_phy_info *phy = &hw->phy;
	s32 ret_val;
	u16 phy_data, length;

	DEBUGFUNC("e1000_get_cable_length_82577");

	ret_val = phy->ops.read_reg(hw, I82577_PHY_DIAG_STATUS, &phy_data);
	if (ret_val)
		return ret_val;

	length = ((phy_data & I82577_DSTATUS_CABLE_LENGTH) >>
		  I82577_DSTATUS_CABLE_LENGTH_SHIFT);

	if (length == E1000_CABLE_LENGTH_UNDEFINED)
		return -E1000_ERR_PHY;

	phy->cable_length = length;

	return E1000_SUCCESS;
}

/**
 *  e1000_write_phy_reg_gs40g - Write GS40G  PHY register
 *  @hw: pointer to the HW structure
 *  @offset: register offset to write to
 *  @data: data to write at register offset
 *
 *  Acquires semaphore, if necessary, then writes the data to PHY register
 *  at the offset.  Release any acquired semaphores before exiting.
 **/
s32 e1000_write_phy_reg_gs40g(struct e1000_hw *hw, u32 offset, u16 data)
{
	s32 ret_val;
	u16 page = offset >> GS40G_PAGE_SHIFT;

	DEBUGFUNC("e1000_write_phy_reg_gs40g");

	offset = offset & GS40G_OFFSET_MASK;
	ret_val = hw->phy.ops.acquire(hw);
	if (ret_val)
		return ret_val;

	ret_val = e1000_write_phy_reg_mdic(hw, GS40G_PAGE_SELECT, page);
	if (ret_val)
		goto release;
	ret_val = e1000_write_phy_reg_mdic(hw, offset, data);

release:
	hw->phy.ops.release(hw);
	return ret_val;
}

/**
 *  e1000_read_phy_reg_gs40g - Read GS40G  PHY register
 *  @hw: pointer to the HW structure
 *  @offset: lower half is register offset to read to
 *     upper half is page to use.
 *  @data: data to read at register offset
 *
 *  Acquires semaphore, if necessary, then reads the data in the PHY register
 *  at the offset.  Release any acquired semaphores before exiting.
 **/
s32 e1000_read_phy_reg_gs40g(struct e1000_hw *hw, u32 offset, u16 *data)
{
	s32 ret_val;
	u16 page = offset >> GS40G_PAGE_SHIFT;

	DEBUGFUNC("e1000_read_phy_reg_gs40g");

	offset = offset & GS40G_OFFSET_MASK;
	ret_val = hw->phy.ops.acquire(hw);
	if (ret_val)
		return ret_val;

	ret_val = e1000_write_phy_reg_mdic(hw, GS40G_PAGE_SELECT, page);
	if (ret_val)
		goto release;
	ret_val = e1000_read_phy_reg_mdic(hw, offset, data);

release:
	hw->phy.ops.release(hw);
	return ret_val;
}

/**
 *  e1000_read_phy_reg_mphy - Read mPHY control register
 *  @hw: pointer to the HW structure
 *  @address: address to be read
 *  @data: pointer to the read data
 *
 *  Reads the mPHY control register in the PHY at offset and stores the
 *  information read to data.
 **/
s32 e1000_read_phy_reg_mphy(struct e1000_hw *hw, u32 address, u32 *data)
{
	u32 mphy_ctrl = 0;
	bool locked = false;
	bool ready;

	DEBUGFUNC("e1000_read_phy_reg_mphy");

	/* Check if mPHY is ready to read/write operations */
	ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;

	/* Check if mPHY access is disabled and enable it if so */
	mphy_ctrl = E1000_READ_REG(hw, E1000_MPHY_ADDR_CTRL);
	if (mphy_ctrl & E1000_MPHY_DIS_ACCESS) {
		locked = true;
		ready = e1000_is_mphy_ready(hw);
		if (!ready)
			return -E1000_ERR_PHY;
		mphy_ctrl |= E1000_MPHY_ENA_ACCESS;
		E1000_WRITE_REG(hw, E1000_MPHY_ADDR_CTRL, mphy_ctrl);
	}

	/* Set the address that we want to read */
	ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;

	/* We mask address, because we want to use only current lane */
	mphy_ctrl = (mphy_ctrl & ~E1000_MPHY_ADDRESS_MASK &
		~E1000_MPHY_ADDRESS_FNC_OVERRIDE) |
		(address & E1000_MPHY_ADDRESS_MASK);
	E1000_WRITE_REG(hw, E1000_MPHY_ADDR_CTRL, mphy_ctrl);

	/* Read data from the address */
	ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;
	*data = E1000_READ_REG(hw, E1000_MPHY_DATA);

	/* Disable access to mPHY if it was originally disabled */
	if (locked)
		ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;
	E1000_WRITE_REG(hw, E1000_MPHY_ADDR_CTRL,
			E1000_MPHY_DIS_ACCESS);

	return E1000_SUCCESS;
}

/**
 *  e1000_write_phy_reg_mphy - Write mPHY control register
 *  @hw: pointer to the HW structure
 *  @address: address to write to
 *  @data: data to write to register at offset
 *  @line_override: used when we want to use different line than default one
 *
 *  Writes data to mPHY control register.
 **/
s32 e1000_write_phy_reg_mphy(struct e1000_hw *hw, u32 address, u32 data,
			     bool line_override)
{
	u32 mphy_ctrl = 0;
	bool locked = false;
	bool ready;

	DEBUGFUNC("e1000_write_phy_reg_mphy");

	/* Check if mPHY is ready to read/write operations */
	ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;

	/* Check if mPHY access is disabled and enable it if so */
	mphy_ctrl = E1000_READ_REG(hw, E1000_MPHY_ADDR_CTRL);
	if (mphy_ctrl & E1000_MPHY_DIS_ACCESS) {
		locked = true;
		ready = e1000_is_mphy_ready(hw);
		if (!ready)
			return -E1000_ERR_PHY;
		mphy_ctrl |= E1000_MPHY_ENA_ACCESS;
		E1000_WRITE_REG(hw, E1000_MPHY_ADDR_CTRL, mphy_ctrl);
	}

	/* Set the address that we want to read */
	ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;

	/* We mask address, because we want to use only current lane */
	if (line_override)
		mphy_ctrl |= E1000_MPHY_ADDRESS_FNC_OVERRIDE;
	else
		mphy_ctrl &= ~E1000_MPHY_ADDRESS_FNC_OVERRIDE;
	mphy_ctrl = (mphy_ctrl & ~E1000_MPHY_ADDRESS_MASK) |
		(address & E1000_MPHY_ADDRESS_MASK);
	E1000_WRITE_REG(hw, E1000_MPHY_ADDR_CTRL, mphy_ctrl);

	/* Read data from the address */
	ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;
	E1000_WRITE_REG(hw, E1000_MPHY_DATA, data);

	/* Disable access to mPHY if it was originally disabled */
	if (locked)
		ready = e1000_is_mphy_ready(hw);
	if (!ready)
		return -E1000_ERR_PHY;
	E1000_WRITE_REG(hw, E1000_MPHY_ADDR_CTRL,
			E1000_MPHY_DIS_ACCESS);

	return E1000_SUCCESS;
}

/**
 *  e1000_is_mphy_ready - Check if mPHY control register is not busy
 *  @hw: pointer to the HW structure
 *
 *  Returns mPHY control register status.
 **/
bool e1000_is_mphy_ready(struct e1000_hw *hw)
{
	u16 retry_count = 0;
	u32 mphy_ctrl = 0;
	bool ready = false;

	while (retry_count < 2) {
		mphy_ctrl = E1000_READ_REG(hw, E1000_MPHY_ADDR_CTRL);
		if (mphy_ctrl & E1000_MPHY_BUSY) {
			usec_delay(20);
			retry_count++;
			continue;
		}
		ready = true;
		break;
	}

	if (!ready)
		DEBUGOUT("ERROR READING mPHY control register, phy is busy.\n");

	return ready;
}