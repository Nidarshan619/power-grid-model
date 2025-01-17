# SPDX-FileCopyrightText: 2022 Contributors to the Power Grid Model project <dynamic.grid.calculation@alliander.com>
#
# SPDX-License-Identifier: MPL-2.0

"""
Definition of integers used by the calculation core
"""

# define internal index integer
from ctypes import c_int32, c_int64

import numpy as np

IdxC = c_int64
IdxNp = np.int64
IdC = c_int32
IdNp = np.int32
