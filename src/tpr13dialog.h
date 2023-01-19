#ifndef TPR13DIALOG_H
#define TPR13DIALOG_H

#include <QDialog>
#include <QObject>

namespace Ui {
    class TPR13Dialog;
}

class TPR13Dialog : public QDialog {
Q_OBJECT
signals:

    void GetTPR13Data(uint32_t value);

public:
    explicit TPR13Dialog(QWidget *parent = nullptr);

    ~TPR13Dialog() override;

    void SetTPR13Value(uint32_t value);

private slots:

    void on_buttonBox_accepted();

private:
    void updateTPR13Value();

    void updateTPR13UIValue();

private:
    typedef struct dram_tpr13_t {
        uint8_t dram_size_auto_det: 1;
        uint8_t timing_auto_det: 1;
        uint8_t dqs_gate_mode_1: 1;
        uint8_t dqs_gate_mode_2: 1;
        uint8_t type_auto_det_mode: 1;
        uint8_t enable_2t_mode: 1;
        uint8_t pll_src_sel: 1;
        uint8_t dbg_0: 1;
        uint8_t vtf_enable: 1;
        uint8_t dynamic_clk: 1;
        uint8_t mdfs_mode: 1;
        uint8_t mdfs_enable: 1;
        uint8_t mdfs_odt_mode: 1;
        uint8_t dramtyp_det_enable: 1;
        uint8_t rnk_wth_auto_det: 1;
        uint8_t boot_auto_det: 1;
        uint8_t cfs_ssc_step_0: 1;
        uint8_t cfs_ssc_step_1: 1;
        uint8_t cfs_ssc_step_2: 1;
        uint8_t cfs_ssc_step_3: 1;
        uint8_t ssc_factor_0: 1;
        uint8_t ssc_factor_1: 1;
        uint8_t ssc_factor_2: 1;
        uint8_t rfu_0: 1;
        uint8_t rfu_1: 1;
        uint8_t rfu_2: 1;
        uint8_t dq_hold_disable: 1;
        uint8_t mst_prio_disable: 1;
        uint8_t rfu_3: 1;
        uint8_t rfu_4: 1;
        uint8_t rfu_5: 1;
        uint8_t rfu_6: 1;
    } dram_tpr13_t;

    Ui::TPR13Dialog *ui;
    uint32_t TPRConfigValue = 0x00000000;
    dram_tpr13_t dramTpr13Value{};
};

#endif // TPR13DIALOG_H
