// Copyright (c) 2020 National Instruments
// SPDX-License-Identifier: MIT

import createVireoCore from '../../dist/wasm32-unknown-emscripten/profile/vireo.core.js';
import createInstanceUnbound from './vireo.loader.shared.js';
import staticHelpers from './vireo.loader.staticHelpers.js';

const createInstance = createInstanceUnbound.bind(undefined, createVireoCore);
export default {
    createInstance,
    staticHelpers
};
