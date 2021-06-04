const addon = require('../build/Release/addon')

test('test #1', () =>
{
    var obj = {
        toJSON(key) {
            return 'foobar'
        }
    }

    expect(addon.toJSON(obj)).toBe('foobar')
})

test('test #2', () =>
{
    var obj = {
        foo: 1.0,
        toJSON(key) {
            return this
        }
    }

    expect(addon.toJSON(obj)).toHaveProperty('foo')
})

test('test #3', () =>
{
    var Foo = function(name)
    {
        this.name = name
    }

    Foo.prototype.toJSON = function() { return ['Foo', this.name] }

    var Bar = function(name)
    {
        Foo.call(this, name)
    }

    Bar.prototype = new Foo()

    var b = new Bar('bar')

    expect(addon.toJSON(b)).toEqual(['Foo', 'bar'])
})

test('test #4', () =>
{
    BigInt.prototype.toJSON = function() { return this.toString() }

    expect(JSON.stringify(BigInt('123'))).toBe('"123"') // this is good

    expect(addon.toJSON(BigInt('123'))).toBe('123')
})

