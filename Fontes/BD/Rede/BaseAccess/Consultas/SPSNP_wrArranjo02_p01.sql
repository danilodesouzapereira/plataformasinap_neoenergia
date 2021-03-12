    INSERT INTO
      Arranjo_02_04 ( fltZ0R, fltZ0X, fltZ1R, fltZ1X, fltC0, fltC1, fltIadm, intSuporteId, intArranjoId )
    VALUES
      ( [p_Z0R], [p_Z0X], [p_Z1R], [p_Z1X], [p_C0], [p_C1], [p_Iadm], IIF([p_SuporteId] <> -1, [p_SuporteId], NULL), [p_ArranjoId] );
