/*
 * Source: http://www.jroller.com/DhavalDalal/entry/equals_hashcode_and_tostring_builders
 *
 * Licensed under Creative Commons 3.0.
 */

using System;
using System.Linq.Expressions;

internal class HashCodeBuilder<T>
{
    private readonly T target;
    private int hashCode = 17;

    public HashCodeBuilder(T target)
    {
        this.target = target;
    }

    public HashCodeBuilder<T> With<TProperty>(Expression<Func<T, TProperty>> propertyOrField)
    {
        var expression = propertyOrField.Body as MemberExpression;
        if (expression == null)
        {
            throw new ArgumentException("Expecting Property or Field Expression of an object");
        }
        var func = propertyOrField.Compile();
        var value = func(target);
        hashCode += 31 * hashCode + ((value == null) ? 0 : value.GetHashCode());
        return this;
    }

    public int HashCode
    {
        get
        {
            return hashCode;
        }
    }
}
