.. _ug_radio_fem:

Radio Front-end Module (FEM) support
####################################

|NCS| allows you to extend the radio range of your board with an implementation of the Front-End Modules (FEM).
The FEM support is based on the :ref:`nrfxlib:mpsl_fem`, which is integrated in nrfxlib's MPSL library.
This library provides nRF21540 GPIO and Simple GPIO implementations, for 3-pin and 2-pin PA/LNA interfaces, respectively.

This guide describes how to add support for these FEM implementations to your application in |NCS|:

* :ref:`ug_radio_fem_nrf21540_gpio` - For the nRF21540 GPIO implementation that uses nRF21540.
* :ref:`ug_radio_fem_skyworks` - For the Simple GPIO implementation that uses the SKY66112-11 device.

These methods are only available to protocol drivers that are using FEM features provided by the MPSL library in multiprotocol scenarios.
They are also valid for cases where an application uses just one protocol, but benefits from features provided by MPSL.
To avoid conflicts, check the protocol documentation to see if it uses FEM support provided by MPSL.

Work is underway to make the protocols shipped with |NCS| use FEM, but none of them currently have this feature.

|NCS| provides a friendly wrapper that configures FEM based on Devicetree (DTS) and Kconfig information.
To enable FEM support, you must enable FEM and MPSL, and add an ``nrf_radio_fem`` node in the Devicetree file.
The node can also be provided by the target board Devicetree file or by an overlay file.
See :ref:`zephyr:dt-guide` for more information about the DTS data structure, and :ref:`zephyr:dt_vs_kconfig` for information about differences between DTS and Kconfig.

.. _ug_radio_fem_requirements:

Enabling FEM and MPSL
*********************

Before you add the Devicetree node in your application, complete the following steps:

1. Add support for the MPSL library in your application.
   The MPSL library provides API to configure FEM.
   See :ref:`nrfxlib:mpsl_lib` in the nrfxlib documentation for details.
#. Enable support for MPSL implementation in |NCS| by setting the :option:`CONFIG_MPSL` Kconfig option to ``y``.

.. _ug_radio_fem_nrf21540_gpio:

Adding support for nRF21540 in GPIO mode
****************************************

The nRF21540 device is a range extender that can be used with the nRF52 and nRF53 Series devices.
For more information about this device, see the `nRF21540`_ documentation.

The nRF21540 GPIO mode implementation of FEM is compatible with this device and implements the 3-pin PA/LNA interface.

To use nRF21540 in GPIO mode, complete the following steps:

1. Add the following node in the Devicetree file:

.. code-block::

   / {
       nrf_radio_fem: name_of_fem_node {
           compatible = "nordic,nrf21540_gpio";
           tx-en-pin  = < 13 >;
           rx-en-pin  = < 14 >;
           pdn-pin    = < 15 >;
       };
   };

#. Replace the node name ``name_of_fem_node``.
#. Replace the pin numbers provided for each of the required properties:

   * ``tx-en-pin`` - Pin number of the device that controls the ``TX_EN`` signal of nRF21540.
   * ``rx-en-pin`` - Pin number of the device that controls the ``RX_EN`` signal of nRF21540.
   * ``pdn-pin`` - Pin number of the device that controls the ``PDN`` signal of nRF21540.

   These properties correspond to ``TX_EN``, ``RX_EN``, and ``PDN`` pins of nRF21540 that are supported by software FEM.
   State of other control pins should be set by other means and according to `nRF21540 Objective Product Specification`_.
   
   In the naming convention used in the API of the MPSL library, the functionalities designated as ``PA`` and ``LNA`` correspond to the above-mentioned pins ``TX_EN`` and ``RX_EN`` respectively.

#. Set the following Kconfig parameters to assign unique GPIOTE channel numbers to be used exclusively by the FEM driver:

   * :option:`MPSL_FEM_NRF21540_GPIO_GPIOTE_TX_EN`
   * :option:`MPSL_FEM_NRF21540_GPIO_GPIOTE_RX_EN`
   * :option:`MPSL_FEM_NRF21540_GPIO_GPIOTE_PDN`

#. Set the following Kconfig parameters to assign unique PPI channel numbers to be used exclusively by the FEM driver:

   * :option:`MPSL_FEM_NRF21540_GPIO_PPI_CHANNEL_0`
   * :option:`MPSL_FEM_NRF21540_GPIO_PPI_CHANNEL_1`
   * :option:`MPSL_FEM_NRF21540_GPIO_PPI_CHANNEL_2`

Optional properties
===================

The following properties are optional and can be added to the Devicetree node if needed:

* Properties that control the timing of interface signals:

  * ``tx-en-settle-time-us`` - Minimal time interval between asserting the ``TX_EN`` signal and start of the radio transmission, in microseconds.
  * ``rx-en-settle-time-us`` - Minimal time interval between asserting the ``RX_EN`` signal and start of the radio transmission, in microseconds.

    .. important::
        Values for these two properties cannot be higher than the Radio Ramp-Up time defined by :c:macro:`TX_RAMP_UP_TIME` and :c:macro:`RX_RAMP_UP_TIME`.
        If the value is too high, the radio driver will not work properly and will not control FEM.
        Moreover, setting a value that is lower than the default value can cause disturbances in the radio transmission, because FEM may be triggered too late.

  * ``pdn-settle-time-us`` - Time interval before the PA or LNA activation reserved for the FEM ramp-up, in microseconds.
  * ``trx-hold-time-us`` - Time interval for which the FEM is kept powered up after the event that triggers the PDN deactivation, in microseconds.

  The default values of these properties are appropriate for default hardware and most use cases.
  You can override them if you need additional capacitors, for example when using custom hardware.
  In such cases, add the property name under the required properties in the device tree node and set a new custom value.

  .. note::
    These values have some constraints.
    For details, see `nRF21540 Objective Product Specification`_.

* Properties that inform protocol drivers about gains provided by nRF21540:

  * ``tx-gain-db`` - Transmission gain value in dB.
  * ``rx-gain-db`` - Reception gain value in dB.

  .. note::
        These properties are not currently implemented.

.. _ug_radio_fem_skyworks:

Adding support for SKY66112-11
******************************

SKY66112-11 is one of many FEM devices that support the 2-pin PA/LNA interface.

To use the Simple GPIO implementation of FEM with SKY66112-11, complete the following steps:

1. Add the following node in the Devicetree file:

.. code-block::

   / {
       nrf_radio_fem: skyworks_shield {
           compatible = "skyworks,sky66112-11";
           ctx-pin = < 13 >;
           crx-pin = < 14 >;
       };
   };

#. Replace the pin numbers provided for each of the required properties:

   * ``ctx-pin`` - Pin number of a device that controls the ``CTX`` signal of SKY66112-11.
   * ``crx-pin`` - Pin number of a device that controls the ``CRX`` signal of SKY66112-11.

   These properties correspond to ``CTX`` and ``CRX`` pins of SKY66112-11 that are supported by software FEM.
   State of other control pins should be set according to SKY66112-11 documentation.
   See the official `SKY66112-11 page`_ for more information.

   In the naming convention used in the API of the MPSL library, the functionalities designated as ``PA`` and ``LNA`` correspond to the above-mentioned pins ``CTX`` and ``CRX`` respectively.
#. Set the following Kconfig parameters to assign unique GPIOTE channel numbers to be used exclusively by the FEM driver:

   * :option:`MPSL_FEM_SKY66112_11_GPIOTE_CTX`
   * :option:`MPSL_FEM_SKY66112_11_GPIOTE_CRX`

#. Set the following Kconfig parameters to assign unique PPI channel numbers to be used exclusively by the FEM driver:

   * :option:`MPSL_FEM_NRF21540_GPIO_PPI_CHANNEL_0`
   * :option:`MPSL_FEM_NRF21540_GPIO_PPI_CHANNEL_1`

Optional properties
===================

The following properties are optional and can be added to the Devicetree node if needed:

* Properties that control the timing of interface signals:

  * ``ctx-settle-time-us`` - Minimal time interval between asserting the ``CTX`` signal and start of the radio transmission, in microseconds.
  * ``crx-settle-time-us`` - Minimal time interval between asserting the ``CRX`` signal and start of the radio transmission, in microseconds.

  The default values of these properties are appropriate for default hardware and most use cases.
  You can override them if you need additional capacitors, for example when using custom hardware.
  In such cases, add the property name under the required properties in the device tree node and set a new custom value.

  .. note::
    These values have some constraints.
    For details, see the official documentation at the `SKY66112-11 page`_.

* Properties that inform protocol drivers about gains provided by SKY66112-11:

  * ``tx-gain-db`` - Transmission gain value in dB.
  * ``rx-gain-db`` - Reception gain value in dB.

  The default values are accurate for SKY66112-11, but can be overridden when using similar device with different gain.
