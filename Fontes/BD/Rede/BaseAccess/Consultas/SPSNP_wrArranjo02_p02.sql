    UPDATE
      (SELECT * FROM Arranjo_02_04) sel
    SET
      sel.fltZ0R       = [p_Z0R],
      sel.fltZ0X       = [p_Z0X],
      sel.fltZ1R       = [p_Z1R],
      sel.fltZ1X       = [p_Z1X],
      sel.fltC0        = [p_C0],
      sel.fltC1        = [p_C1],
      sel.fltIadm      = [p_Iadm],
      sel.intSuporteId = IIF([p_SuporteId] <> -1, [p_SuporteId], NULL)
    WHERE
      sel.intArranjoId = [p_ArranjoId];
