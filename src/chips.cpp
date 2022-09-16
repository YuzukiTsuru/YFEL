#include "chips.h"

chip_function_e Chips::chip_detect()
{
    if(chip_info.chip_id == 0x00000000)
        return chip_function_e::Success;
    return chip_function_e::Fail;
}

chip_function_e Chips::chip_reset()
{
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_sid(uint8_t sid)
{
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_jtag()
{
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_ddr(chip_ddr_type_e dram_type)
{
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_spi_init(uint32_t *swapbuf, uint32_t *swaplen, uint32_t *cmdlen)
{
    return chip_function_e::NotSupport;
}

chip_function_e Chips::chip_spi_run(uint8_t *cbuf, uint32_t clen)
{
    return chip_function_e::NotSupport;
}

chip_t Chips::get_chip_info()
{
    return chip_info;
}
