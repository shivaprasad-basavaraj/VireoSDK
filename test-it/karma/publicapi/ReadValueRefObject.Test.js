// Copyright (c) 2020 National Instruments
// SPDX-License-Identifier: MIT

describe('The Vireo EggShell readValueRefObject', function () {
    'use strict';

    // Reference aliases
    var vireoHelpers = window.vireoHelpers;
    var vireoRunner = window.testHelpers.vireoRunner;
    var fixtures = window.testHelpers.fixtures;

    var vireo;
    beforeAll(async function () {
        vireo = await vireoHelpers.createInstance();
    });

    afterAll(function () {
        vireo = undefined;
    });

    var publicApiMultipleTypesViaUrl = fixtures.convertToAbsoluteFromFixturesDir('publicapi/MultipleTypes.via');
    var viName = 'MyVI';

    var expectValidValueRef = function (valueRef) {
        expect(valueRef).toBeNonEmptyObject();
        expect(valueRef.typeRef).toBeNumber();
        expect(valueRef.typeRef).not.toBe(0);
        expect(valueRef.dataRef).toBeNumber();
        expect(valueRef.dataRef).not.toBe(0);
    };

    beforeAll(function (done) {
        fixtures.preloadAbsoluteUrls([
            publicApiMultipleTypesViaUrl
        ], done);
    });

    beforeAll(function () {
        vireoRunner.rebootAndLoadVia(vireo, publicApiMultipleTypesViaUrl);
    });

    describe('for a cluster of scalars', function () {
        var valueRef,
            clusterOfScalarsPath = 'dataItem_ClusterOfScalars';

        beforeAll(function () {
            valueRef = vireo.eggShell.findValueRef(viName, clusterOfScalarsPath);
        });

        it('returns an object with field names as keys and valueRefs as values', function () {
            var objectRef = vireo.eggShell.readValueRefObject(valueRef);
            expectValidValueRef(objectRef.bool);
            expectValidValueRef(objectRef.string);
            expectValidValueRef(objectRef.double);
            expectValidValueRef(objectRef.int32);
            expectValidValueRef(objectRef.int64);
            expectValidValueRef(objectRef.uint64);
            expectValidValueRef(objectRef.complex);
            expectValidValueRef(objectRef.time);
        });

        it('valueRefObject can be used to read from it', function () {
            var objectValueRef = vireo.eggShell.readValueRefObject(valueRef);
            expect(vireo.eggShell.readDouble(objectValueRef.bool)).toBe(1);
            expect(vireo.eggShell.readDouble(objectValueRef.double)).toMatchIEEE754Number(3.14159);
            expect(vireo.eggShell.readDouble(objectValueRef.int32)).toBe(42);
            expect(vireo.eggShell.readDouble(objectValueRef.int64)).toBe(-72057594037927936);
            expect(vireo.eggShell.readDouble(objectValueRef.uint64)).toBe(9223372041149743104);
            expect(vireo.eggShell.readDouble(objectValueRef.time)).toBe(3564057536.423476);
        });
    });

    describe('for a cluster of scalars with encoded field names', function () {
        var valueRef,
            clusterOfScalarsPath = 'dataItem_ClusterOfEncodedScalars';

        beforeAll(function () {
            valueRef = vireo.eggShell.findValueRef(viName, clusterOfScalarsPath);
        });

        it('returns an object with decoded field names as keys', function () {
            var objectRef = vireo.eggShell.readValueRefObject(valueRef);
            expect(objectRef['bool fun']).toBeDefined();
            expect(objectRef['string fun']).toBeDefined();
            expect(objectRef['double fun']).toBeDefined();
            expect(objectRef['int32 fun']).toBeDefined();
            expect(objectRef['int64 fun']).toBeDefined();
            expect(objectRef['uint64 fun']).toBeDefined();
            expect(objectRef['complex fun']).toBeDefined();
            expect(objectRef['time fun']).toBeDefined();
        });
    });

    describe('for a timestamp', function () {
        var valueRef,
            timestampPath = 'dataItem_Timestamp';

        beforeAll(function () {
            valueRef = vireo.eggShell.findValueRef(viName, timestampPath);
        });

        it('returns an object with keys "seconds" and "fraction" and valueRefs as values', function () {
            var objectValueRef = vireo.eggShell.readValueRefObject(valueRef);
            expectValidValueRef(objectValueRef.seconds);
            expectValidValueRef(objectValueRef.fraction);
        });
    });

    describe('for an analogwaveform', function () {
        var valueRef,
            analogWaveformPath = 'wave_Double';

        beforeAll(function () {
            valueRef = vireo.eggShell.findValueRef(viName, analogWaveformPath);
        });

        it('returns an object with keys "dt", "t0" and "Y" and valueRefs as values', function () {
            var objectValueRef = vireo.eggShell.readValueRefObject(valueRef);
            expectValidValueRef(objectValueRef.t0);
            expectValidValueRef(objectValueRef.dt);
            expectValidValueRef(objectValueRef.Y);
        });
    });

    describe('for complex', function () {
        var valueRef,
            complexSinglePath = 'dataItem_ComplexSingle';

        beforeAll(function () {
            valueRef = vireo.eggShell.findValueRef(viName, complexSinglePath);
        });

        it('returns an object with keys "real" and "imaginary" and valueRefs as values', function () {
            var objectValueRef = vireo.eggShell.readValueRefObject(valueRef);
            expectValidValueRef(objectValueRef.real);
            expectValidValueRef(objectValueRef.imaginary);
        });
    });

    describe('for path', function () {
        var valueRef,
            nipathPath = 'win32Path';

        beforeAll(function () {
            valueRef = vireo.eggShell.findValueRef(viName, nipathPath);
        });

        it('returns an object with keys "components" and "type" and valueReds as values', function () {
            var objectValueRef = vireo.eggShell.readValueRefObject(valueRef);
            expectValidValueRef(objectValueRef.components);
            expectValidValueRef(objectValueRef.type);
        });
    });

    it('throws for any other type', function () {
        var tryReadValueRefObject = function (path) {
            return function () {
                vireo.eggShell.readValueRefObject(vireo.eggShell.findValueRef(viName, path));
            };
        };

        expect(tryReadValueRefObject('dataItem_ArrayOfBoolean')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('dataItem_ArrayOfClusters')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('dataItem_NumericSingle')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('dataItem_NumericDouble')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('booleanTrueValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('int8MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('int16MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('int32MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('int64MinSafeInteger')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('int64MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('uInt8MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('uInt16MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('uInt32MinValue')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('uInt64MinSafeInteger')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('dataItem_String')).toThrowError(/ValueRefObject/);
        expect(tryReadValueRefObject('enum8alphabet')).toThrowError(/ValueRefObject/);
    });
});
