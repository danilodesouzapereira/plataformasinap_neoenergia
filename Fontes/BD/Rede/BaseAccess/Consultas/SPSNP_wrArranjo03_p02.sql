    UPDATE
      (SELECT * FROM Arranjo_03)
    SET
      sel.blbMatZ = [p_MatZ],
      sel.blbMatC = [p_MatC],
      sel.fltIadm = [p_Iadm]
    WHERE
      sel.intArranjoId = [p_ArranjoId];
