# SPDX-FileCopyrightText: 2022 Contributors to the Power Grid Model project <dynamic.grid.calculation@alliander.com>
#
# SPDX-License-Identifier: MPL-2.0


"""
Error classes
"""

from power_grid_model.power_grid_core import power_grid_core as pgc
from typing import List, Optional


class PowerGridError(Exception):
    pass


class PowerGridBatchError(Exception):
    failed_scenarios: List[int]
    error_messages: List[str]


def find_error() -> Optional[Exception]:
    """

    Returns:

    """
    error_code: int = pgc.err_code()
    if error_code == 0:
        return None
    elif error_code == 1:
        error_message = pgc.err_msg()
        error_message += "\nTry validate_input_data() or validate_batch_data() to validate your data.\n"
        return PowerGridError(error_message)
    elif error_code == 2:
        error_message = (
            "There are errors in the batch calculation.\nTry validate_input_data() or "
            "validate_batch_data() to validate your data.\n"
        )
        error = PowerGridBatchError(error_message)
        n_fails = pgc.n_failed_batches()
        failed_idxptr = pgc.failed_batches()
        failed_msgptr = pgc.batch_errs()
        error.failed_scenarios = [failed_idxptr[i] for i in range(n_fails)]
        error.error_messages = [failed_msgptr[i].decode() for i in range(n_fails)]
        return error
    else:
        return Exception("Unknown error!")


def assert_error():
    """

    Returns:

    """
    error = find_error()
    if error is not None:
        raise error
